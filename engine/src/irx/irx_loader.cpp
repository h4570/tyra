/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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

// external IRX modules
extern u8 bdm_irx[];
extern int size_bdm_irx;

extern u8 bdmfs_fatfs_irx[];
extern int size_bdmfs_fatfs_irx;

extern u8 usbd_irx[];
extern int size_usbd_irx;

extern u8 usbmass_bd_irx[];
extern int size_usbmass_bd_irx;

namespace Tyra {

IrxLoader::IrxLoader() {
  SifInitRpc(0);
  this->applyRpcPatches();
}

IrxLoader::~IrxLoader() {}

void IrxLoader::loadDefaultDrivers() {
  this->loadAudio();
  this->loadPad();
}

void IrxLoader::loadUSBDriver() { this->loadUsb(); }

/**
 * @brief Apply the SBV LMB patch to allow modules to be loaded from a buffer in
 * EE RAM.
 *
 */
int IrxLoader::applyRpcPatches() {
  int ret;
  TYRA_LOG("Applying SBV Patches");

  ret = sbv_patch_enable_lmb();
  TYRA_ASSERT(ret >= 0,
              "Failed to load Applying SBV Patches sbv_patch_enable_lmb");

  ret = sbv_patch_disable_prefix_check();
  TYRA_ASSERT(
      ret >= 0,
      "Failed to load Applying SBV Patches sbv_patch_disable_prefix_check");

  ret = sbv_patch_fileio();
  TYRA_ASSERT(ret >= 0, "Failed to load Applying SBV Patches sbv_patch_fileio");

  TYRA_LOG("SBV Patches applyed ");
  return ret;
}

int IrxLoader::loadUsb() {
  int ret;
  TYRA_LOG("Loading USB modules");

  // Load Block Device Manager (BDM)
  SifExecModuleBuffer(&bdm_irx, size_bdm_irx, 0, NULL, &ret);
  TYRA_ASSERT(ret >= 0, "Failed to load module: usbhdfsd");

  // Load FATFS (mass:) driver
  SifExecModuleBuffer(&bdmfs_fatfs_irx, size_bdmfs_fatfs_irx, 0, NULL, &ret);
  TYRA_ASSERT(ret >= 0, "Failed to load module: usbd");

  // Load USB Block Device drivers
  SifExecModuleBuffer(&usbd_irx, size_usbd_irx, 0, NULL, &ret);
  TYRA_ASSERT(ret >= 0, "Failed to load module: usbd");

  SifExecModuleBuffer(&usbmass_bd_irx, size_usbmass_bd_irx, 0, NULL, &ret);
  TYRA_ASSERT(ret >= 0, "Failed to load module: usbhdfsd");

  this->waitUntilUsbDeviceIsReady();

  TYRA_LOG("USB/MASS modules loaded!");

  return ret;
}

int IrxLoader::loadAudio() {
  int ret;

  TYRA_LOG("Modules loading started (LIBSD, AUDSRV)");
  ret = SifLoadModule("rom0:LIBSD", 0, NULL);
  TYRA_ASSERT(ret != -203, "LIBSD loading failed!");

  ret = SifLoadModule("host:audsrv.irx", 0, NULL);
  TYRA_ASSERT(ret != -203, "audsrv.irx loading failed!");
  TYRA_LOG("Audio modules loaded");

  return ret;
}

int IrxLoader::loadPad() {
  int ret;

  TYRA_LOG("PAD Modules loading started (SIO2MAN, PADMAN)");

  ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
  TYRA_ASSERT(ret >= 0,
              "SifLoadModule (SIO2MAN) failed! Returned value: ", ret);

  ret = SifLoadModule("rom0:PADMAN", 0, NULL);
  TYRA_ASSERT(ret >= 0, "SifLoadModule (PADMAN) failed! Returned value: ", ret);

  TYRA_LOG("Pad modules loaded!");

  return ret;
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
