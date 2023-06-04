#include <kernel.h>
#include <netman.h>
#include <ps2ips.h>
#include <stdio.h>

#pragma once

namespace Tyra {

class TCP
{
public:
TCP();
~TCP();

char *GetIP();
int Init(int use_dhcp, ip4_addr *IP_adrr, ip4_addr *Netmask, ip4_addr *Gateway, ip4_addr *DNS);
int End();
int SetNetIFMode(int mode);
void PrintIPConfig();
int DHCPCheck();

};
}
