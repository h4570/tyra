/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

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

// external IRX modules
#define EXTERN_IRX(_irx) \
  extern u8 _irx[];      \
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

namespace Tyra {

bool IrxLoader::isLoaded = false;

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

std::map<int, std::string> IrxLoader::IOPErrors = {
    {-1, "Unknown error"},
    {-101, "Illegal intrcode"},
    {-102, "CPU ID"},
    {-103, "Interrupt disabled"},
    {-104, "Found handler"},
    {-105, "Handler not found"},
    {-150, "No timer"},
    {-151, "Illegal timer ID"},
    {-152, "Illegal source"},
    {-153, "Illegal prescale"},
    {-154, "Timer busy"},
    {-155, "Timer not setup"},
    {-156, "Timer not in use"},
    {-160, "Unit used"},
    {-161, "Unit not used"},
    {-162, "No ROMDIR"},
    {-200, "Linker error (missing driver dependency/imports)"},
    {-201, "Illegal object"},
    {-202, "Unknown Module"},
    {-203, "No such file"},
    {-204, "File error"},
    {-205, "Memory in use"},
    {-206, "Already started"},
    {-207, "Not started"},
    {-208, "Module already stopped"},
    {-209, "Cannot stop module"},
    {-210, "Module not stopped"},
    {-211, "Module not removable"},
    {-212, "Library found"},
    {-213, "Library not found"},
    {-214, "Illegal library"},
    {-215, "Library in use"},
    {-216, "Already stopping"},
    {-217, "Illegal offset"},
    {-218, "Illegal position"},
    {-219, "Illegal access"},
    {-220, "Illegal flag"},
    {-400, "NO MEMORY"},
    {-401, "Illegal attribute"},
    {-402, "Illegal entry"},
    {-403, "Illegal priority"},
    {-404, "Illegal stack size"},
    {-405, "Illegal mode"},
    {-406, "Illegal THID"},
    {-407, "Unknown THID"},
    {-408, "Unknown SEMID"},
    {-409, "Unknown EVFID"},
    {-410, "Unknown MBXID"},
    {-411, "Unknown VPLID"},
    {-412, "Unknown FPLID"},
    {-413, "Dormant"},
    {-414, "Not dormant"},
    {-415, "Not suspend"},
    {-416, "Not Waiting"},
    {-417, "Cannot wait"},
    {-418, "KE_RELEASE_WAIT"},
    {-419, "KE_SEMA_ZERO"},
    {-420, "KE_SEMA_OVF"},
    {-421, "KE_EVF_COND"},
    {-422, "KE_EVF_MULTI"},
    {-423, "KE_EVF_ILPAT"},
    {-424, "KE_MBOX_NOMSG"},
    {-425, "Wait delete"},
    {-426, "Illegal memblock"},
    {-427, "Illegal memsize"},
    {-428, "Illegal SPAD addr"},
    {-429, "SPAD in use"},
    {-430, "SPAD not in use"},
    {-431, "Illegal type"},
    {-432, "Illegal size"},
};

/**
 * @brief an equivalent of C standard `strerror()` for the IOP error codes
 * @note although this is intended for checking IRX module error status. it can
 * be used to evaluate return values from any operation requested to the IOP.
 * such as module unload request. simply pass the error number to the ID
 * parameter
 * @param ID module ID or IOP related return value
 * @param RET module return value obtained by the last parameter of
 * `SifExecModuleBuffer`
 * @return description of an IOP error or IRX startup error
 */
std::string IrxLoader::GetIrxErrorDescription(const int ID, const int RET = 0) {
  if (RET == 1) return "Module willingly requested to be unloaded from IOP";
  return IrxLoader::IOPErrors[ID];
}

void IrxLoader::loadAll(const bool& withUsb, const bool& isLoggingToFile) {
  if (isLoaded) {
    TYRA_LOG("IRX modules already loaded!");
    return;
  }

  loadIO(!isLoggingToFile);
  loadSio2man(!isLoggingToFile);
  loadPadman(!isLoggingToFile);
  loadLibsd(!isLoggingToFile);

  if (withUsb) {
    loadUsbModules(!isLoggingToFile);
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

  int ret, id;
  id = SifExecModuleBuffer(&libsd_irx, size_libsd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: libsd_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  if (verbose) TYRA_LOG("IRX: Libsd loaded!");
}

void IrxLoader::loadIO(const bool& verbose) {
  int ret, id;
  if (verbose) TYRA_LOG("IRX: Loading iomanX...");

  id = SifExecModuleBuffer(&iomanX_irx, size_iomanX_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: iomanX_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  if (verbose) TYRA_LOG("IRX: iomanX loaded!");

  if (verbose) TYRA_LOG("IRX: Loading fileXio...");

  SifExecModuleBuffer(&fileXio_irx, size_fileXio_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: fileXio_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  if (verbose) TYRA_LOG("IRX: fileXio_irx loaded!");
}

void IrxLoader::loadUsbModules(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading usb modules...");

  int ret, id;

  id = SifExecModuleBuffer(&usbd_irx, size_usbd_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: usbd_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  id = SifExecModuleBuffer(&bdm_irx, size_bdm_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: bdm_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  id = SifExecModuleBuffer(&bdmfs_fatfs_irx, size_bdmfs_fatfs_irx, 0, nullptr,
                           &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: bdmfs_fatfs_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  id = SifExecModuleBuffer(&usbmass_bd_irx, size_usbmass_bd_irx, 0, nullptr,
                           &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: usbmass_bd_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  waitUntilUsbDeviceIsReady();

  if (verbose) TYRA_LOG("IRX: Usb modules loaded!");
}

void IrxLoader::loadAudsrv(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading audsrv...");

  int ret, id;
  id = SifExecModuleBuffer(&audsrv_irx, size_audsrv_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: audsrv_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  if (verbose) TYRA_LOG("IRX: Audsrv loaded!");
}

void IrxLoader::loadSio2man(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading sio2man...");

  int ret, id;
  id = SifExecModuleBuffer(&sio2man_irx, size_sio2man_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: sio2man_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

  if (verbose) TYRA_LOG("IRX: Sio2man loaded!");
}

void IrxLoader::loadPadman(const bool& verbose) {
  if (verbose) TYRA_LOG("IRX: Loading padman...");

  int ret, id;
  id = SifExecModuleBuffer(&padman_irx, size_padman_irx, 0, nullptr, &ret);
  TYRA_ASSERT(ret == 1 || id < 0, "Failed to load module: padman_irx",
              IrxLoader::GetIrxErrorDescription(id, ret));

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
