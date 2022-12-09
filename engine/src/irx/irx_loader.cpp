/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
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

void IrxLoader::loadAll(const bool& withUsb, const bool &withHdd, const bool &withCdFS, const bool&withMC,const bool& isLoggingToFile) {
  if (isLoaded) {
    TYRA_LOG("IRX modules already loaded!");
    return;
  }

  init_joystick_driver(true);
  init_poweroff_driver();

  if (withUsb) {
      init_usb_driver(!isLoggingToFile);
  }

  if (withHdd) {
      init_hdd_driver(true, !isLoggingToFile);
  }

  if (withCdFS) {
      init_cdfs_driver();
  }
  
  if (withMC) {
     init_memcard_driver(!isLoggingToFile);
  }

  init_audio_driver();

  isLoaded = true;
}

void IrxLoader::UnLoad() {
     deinit_poweroff_driver();	
     deinit_audio_driver();	
     deinit_joystick_driver(false);	
     deinit_usb_driver(false);	
     deinit_cdfs_driver();
     deinit_memcard_driver(true);	
     deinit_hdd_driver(false);	
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
