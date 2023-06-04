/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022 - 2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Br4k2n <no email>
# André Guilherme <andregui17@outlook.com>
*/

#include <netman.h>
#include <ps2ips.h>

#pragma once

namespace Tyra {

class TCP {
 public:
  TCP();
  ~TCP();

  char* GetIP();
  int Init(int use_dhcp, ip4_addr* IP_adrr, ip4_addr* Netmask,
           ip4_addr* Gateway, ip4_addr* DNS);
  int End();
  int SetNetIFMode(int mode);
  void PrintIPConfig();
  int DHCPCheck();
};
}  // namespace Tyra
