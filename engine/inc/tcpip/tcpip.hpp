#pragma once

namespace Tyra {
static char ipAddress[16];
t_ip_info ip_info;
const ip_addr_t *dns_curr;

class TCP
{
public:
char *GetIP();
int Init(int use_dhcp, ip4_addr *IP_adrr, ip4_addr *Netmask, ip4_addr *Gateway, ip4_addr *DNS);
int End();
int SetNetIFMode(int mode);
static void PrintIPConfig();

};
}

