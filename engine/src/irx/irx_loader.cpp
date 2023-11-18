/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022 - 2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/
#define NEWLIB_PORT_AWARE 
#include <fileXio_rpc.h>

#include "irx/irx_loader.hpp"
#include "debug/debug.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <loadfile.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include "file/file_utils.hpp"
#include <libpwroff.h>
#include <hdd-ioctl.h>
#include <io_common.h>
#include <usbhdfsd-common.h>
// external IRX modules
#define EXTERN_IRX(_irx) \
  extern u8 _irx[]; \
  extern int size_##_irx

EXTERN_IRX(sio2man_irx);
EXTERN_IRX(padman_irx);
EXTERN_IRX(audsrv_irx);
EXTERN_IRX(libsd_irx);
EXTERN_IRX(fileXio_irx);
EXTERN_IRX(iomanX_irx);
EXTERN_IRX(bdm_irx);
EXTERN_IRX(bdmfs_fatfs_irx);
EXTERN_IRX(usbd_irx);
EXTERN_IRX(usbmass_bd_irx);
EXTERN_IRX(poweroff_irx);
EXTERN_IRX(ps2hdd_irx);
EXTERN_IRX(ps2fs_irx);
EXTERN_IRX(ps2dev9_irx);
EXTERN_IRX(ps2atad_irx);
namespace Tyra {

bool IrxLoader::isLoaded = false;
bool IrxLoader::have_dev9 = false;

IrxLoader::IrxLoader() {
  SifInitRpc(0);

  while (!SifIopReset("", 0)) {
  };
  while (!SifIopSync()) {
  };

  SifInitRpc(0);

  this->applyRpcPatches();
}

IrxLoader::~IrxLoader() {}

void IrxLoader::loadAll(const bool& withUsb, const bool& withHdd,
                        const bool& isLoggingToFile) {
  if (isLoaded) {
    TYRA_LOG("IRX modules already loaded!");
    return;
  }

  loadFileXio(!isLoggingToFile);
  loadSio2man(!isLoggingToFile);
  loadPadman(!isLoggingToFile);
  loadLibsd(!isLoggingToFile);

  if (withUsb) {
    loadUsbModules(!isLoggingToFile);
  }

    loadPowerOff(!isLoggingToFile);
  if (withHdd) {
    loadHddModules(!isLoggingToFile);
  }

  loadAudsrv(true);

  isLoaded = true;
}

/**
 * @brief Apply the SBV LMB patch to allow modules to be loaded from a buffer in
 * EE RAM.
 *
 */
int IrxLoader::applyRpcPatches() {
  int ret;

  ret = sbv_patch_enable_lmb();
  TYRA_ASSERT(ret >= 0,
              "Failed to load Applying SBV Patches sbv_patch_enable_lmb");

  ret = sbv_patch_disable_prefix_check();
  TYRA_ASSERT(
      ret >= 0,
      "Failed to load Applying SBV Patches sbv_patch_disable_prefix_check");

  ret = sbv_patch_fileio();
  TYRA_ASSERT(ret >= 0, "Failed to load Applying SBV Patches sbv_patch_fileio");

  return ret;
}

void IrxLoader::loadLibsd(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading libsd...");

  int ret, irx_id;
  irx_id = SifExecModuleBuffer(&libsd_irx, size_libsd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: libsd_irx ret:", ret, ", id:", irx_id);

  if (verbose) TYRA_LOG("IRX: Libsd loaded!");
}

void IrxLoader::loadFileXio(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading FileXio...");

  int ret, irx_id;

  irx_id = SifExecModuleBuffer(&iomanX_irx, size_iomanX_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: iomanX_irx ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&fileXio_irx, size_fileXio_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: fileXio_irx ret:", ret, ", id:", irx_id);

  fileXioInit();

  if (verbose) TYRA_LOG("IRX: FileXio loaded!");
}

void IrxLoader::loadUsbModules(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading usb modules...");

  int ret, irx_id;

  irx_id = SifExecModuleBuffer(&usbd_irx, size_usbd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: usbd_irx ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&bdm_irx, size_bdm_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: bdm_irx ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&bdmfs_fatfs_irx, size_bdmfs_fatfs_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: bdmfs_fatfs ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&usbmass_bd_irx, size_usbmass_bd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: usbmass ret:", ret, ", id:", irx_id);

  waitUntilUsbDeviceIsReady();

  if (verbose) TYRA_LOG("IRX: Usb modules loaded!");
}

static void poweroffCallback(void *arg)
{
    fileXioDevctl("pfs:", PDIOC_CLOSEALL, NULL, 0, NULL, 0);
    while (fileXioDevctl("dev9x:", DDIOC_OFF, NULL, 0, NULL, 0) < 0) {};
    // As required by some (typically 2.5") HDDs, issue the SCSI STOP UNIT command to avoid causing an emergency park.
    fileXioDevctl("mass:", USBMASS_DEVCTL_STOP_ALL, NULL, 0, NULL, 0);
    /* Power-off the PlayStation 2. */
    poweroffShutdown();
}

void IrxLoader::loadPowerOff(const bool &verbose) {
  if (verbose) TYRA_LOG("IRX: Loading poweroff modules...");

  int ret, irx_id;

  irx_id = SifExecModuleBuffer(&poweroff_irx, size_poweroff_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: poweroff_irx ret:", ret, ", id:", irx_id);
  poweroffInit();
  poweroffSetCallback(&poweroffCallback, nullptr);
}

/* 
 * @brief processes a standard SCE HDD Path to obtain useful information, splitted and ready to use
 * @param path input path eg: `hdd0:__system:pfs:/osdmain/osdmain.elf`
 * @param mount_path pointer to std::string wich will contain path to mount partition eg: `hdd0:__system`
 * @param pfs_dev pointer to std::string wich will contain the pfs device, useful for IOCTL commands and unmounting the partition eg: `pfs:`
 * @param pfs_path pointer to std::string wich will contain the full path to the desired file, wich will be accessible after mounting eg: `pfs:/osdmain/osdmain.elf`
 * @note you can pass nullptr to any of the 3 final parameters to skip them
 * @return boolean for sucess
*/
static bool digestSceStandardHDDPath(const std::string& path, std::string* mount_path, std::string* pfs_dev, std::string* pfs_path) {
  unsigned int I[3] = {0, 0, 0}; // position of the 3 ':' we are looking for
  int p = 0;
  for (int pos = 0; p < 3; p++, pos++) {
    pos = path.find(':', pos);
    I[p] = pos;
  }
  if (p != 3) return false; // because we need 3 ':' for a valid path...
  if (mount_path != nullptr) *mount_path = path.substr(0, I[1]);
  if (pfs_dev != nullptr) *pfs_dev = path.substr(I[1]+1, I[2]-I[1]);
  if (pfs_path != nullptr) *pfs_path = path.substr(I[1]+1);
  return true;
}

bool IrxLoader::loadDEV9(const bool &verbose)
{
  if(verbose) TYRA_LOG("IRX: Loading DEV9 module");
  
  if(!have_dev9) {
    int irx_id, ret;
    irx_id = SifExecModuleBuffer(&ps2dev9_irx, size_ps2dev9_irx, 0, nullptr, &ret);
    TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: ps2dev9_irx ret:", ret, ", id:", irx_id);
    have_dev9 = true;
  }
  return true;
}

void IrxLoader::loadHddModules(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading Hdd Modules!");

  int ret, irx_id;

  if(!loadDEV9(true))
    TYRA_LOG("IRX: Loading Standalone: ps2dev9.irx");

  irx_id = SifExecModuleBuffer(&ps2atad_irx, size_ps2atad_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: ps2atad.irx ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&ps2hdd_irx, size_ps2hdd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: ps2hdd_irx ret:", ret, ", id:", irx_id);

  irx_id = SifExecModuleBuffer(&ps2fs_irx, size_ps2fs_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: ps2fs_irx ret:", ret, ", id:", irx_id);
  
  if (verbose) TYRA_LOG("IRX: Hdd modules loaded");
}

void IrxLoader::loadAudsrv(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading audsrv...");

  int ret, irx_id;
  irx_id = SifExecModuleBuffer(&audsrv_irx, size_audsrv_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: audsrv_irx ret:", ret, ", id:", irx_id);

  if (verbose) TYRA_LOG("IRX: Audsrv loaded!");
}

void IrxLoader::loadSio2man(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading sio2man...");

  int ret, irx_id;
  irx_id = SifExecModuleBuffer(&sio2man_irx, size_sio2man_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: sio2man_irx ret:", ret, ", id:", irx_id);

  if (verbose) TYRA_LOG("IRX: Sio2man loaded!");
}

void IrxLoader::loadPadman(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading padman...");

  int ret, irx_id;
  irx_id = SifExecModuleBuffer(&padman_irx, size_padman_irx, 0, nullptr, &ret);
  TYRA_ASSERT(((ret != 1) && (irx_id > 0)), "Failed to load module: padman_irx ret:", ret, ", id:", irx_id);

  if (verbose) TYRA_LOG("IRX: Padman loaded!");
}

void IrxLoader::delay(int count) {
  int i;
  int ret;
  for (i = 0; i < count; i++) {
    ret = 0x01000000;
    while (ret--) asm("nop\nnop\nnop\nnop");
  }
}

void IrxLoader::waitUntilUsbDeviceIsReady() {
  struct stat buffer;
  int ret = -1;
  int retries = 50;

  delay(5);  // some delay is required by usb mass storage driver

  while (ret != 0 && retries > 0) {
    ret = stat("mass:/", &buffer);
    /* Wait until the device is ready */
    nopdelay();

    retries--;
  }
}

}  // namespace Tyra
