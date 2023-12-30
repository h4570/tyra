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

#include <kernel.h>
#include "tcpip/tcpip.hpp"
#include "debug/debug.hpp"

namespace Tyra {

static char ipAddress[16];

static void ethStatusCheckCb(s32 alarm_id, u16 time, void* common) {
  iWakeupThread(*(s32*)common);
}

static int ethWaitValidNetState(int (*checkingFunction)(void)) {
  s32 ThreadID, retry_cycles;

  // Wait for a valid network status;
  ThreadID = GetThreadId();
  for (retry_cycles = 0; checkingFunction() == 0;
       retry_cycles++) {  // Sleep for 1000ms.
    SetAlarm(1000 * 16, &ethStatusCheckCb, &ThreadID);
    SleepThread();

    if (retry_cycles >= 10)  // 10s = 10*1000ms
      return -1;
  }

  return 0;
}

static int ethGetNetIFLinkStatus(void) {
  return (NetManIoctl(NETMAN_NETIF_IOCTL_GET_LINK_STATUS, NULL, 0, NULL, 0) ==
          NETMAN_NETIF_ETH_LINK_STATE_UP);
}

static int ethWaitValidNetIFLinkState(void) {
  return ethWaitValidNetState(&ethGetNetIFLinkStatus);
}

static int ethGetDHCPStatus() {
  int result;
  t_ip_info ip_info;

  if ((result = ps2ip_getconfig((char*)"sm0", &ip_info)) >=
      0) {  // Check for a successful state if DHCP is enabled.
    if (ip_info.dhcp_enabled)
      result = (ip_info.dhcp_status == DHCP_STATE_BOUND ||
                (ip_info.dhcp_status == DHCP_STATE_OFF));
    else
      result = -1;
  }

  return result;
}

static int ethWaitValidDHCPState(void) {
  return ethWaitValidNetState(&ethGetDHCPStatus);
}

TCP::TCP() {}

TCP::~TCP() { End(); }

char* TCP::GetIP() { return ipAddress; }

int SetNetIFMode(int mode) {
  int result;
  // By default, auto-negotiation is used.
  static int CurrentMode = NETMAN_NETIF_ETH_LINK_MODE_AUTO;

  if (CurrentMode != mode) {  // Change the setting, only if different.
    if ((result = NetManSetLinkMode(mode)) == 0) CurrentMode = mode;
  } else
    result = 0;

  return result;
}

int TCP::Init(int use_dhcp, ip4_addr* IP_adrr, ip4_addr* Netmask,
              ip4_addr* Gateway, ip4_addr* DNS) {
  int result;
  t_ip_info ip_info;
  const ip_addr_t* dns_curr;
  TYRA_LOG("Started TCP Init");

  // SMAP is registered as the "sm0" device to the TCP/IP stack.
  if ((result = ps2ip_getconfig((char*)"sm0", &ip_info)) >= 0) {
    // Obtain the current DNS server settings.
    dns_curr = dns_getserver(0);

    // Check if it's the same. Otherwise, apply the new configuration.
    if ((use_dhcp != static_cast<int>(ip_info.dhcp_enabled)) ||
        (!use_dhcp &&
         (!ip_addr_cmp(IP_adrr,
                       reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr)) ||
          !ip_addr_cmp(Netmask,
                       reinterpret_cast<ip4_addr_t*>(&ip_info.netmask)) ||
          !ip_addr_cmp(Gateway, reinterpret_cast<ip4_addr_t*>(&ip_info.gw)) ||
          !ip_addr_cmp(DNS, dns_curr)))) {
      if (use_dhcp) {
        ip_info.dhcp_enabled = 1;
      } else {  // Copy over new settings if DHCP is not used.
        ip_addr_set(reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr), IP_adrr);
        ip_addr_set(reinterpret_cast<ip4_addr_t*>(&ip_info.netmask), Netmask);
        ip_addr_set(reinterpret_cast<ip4_addr_t*>(&ip_info.gw), Gateway);

        ip_info.dhcp_enabled = 0;
      }

      result = ps2ip_setconfig(&ip_info);
      if (!use_dhcp) dns_setserver(0, reinterpret_cast<ip4_addr_t*>(DNS));
    } else
      result = 0;
  }

  return result;
}

void TCP::PrintIPConfig() {
  u8 ip_address[4], netmask[4], gateway[4], dns[4];
  t_ip_info ip_info;
  const ip_addr_t* dns_curr;
  // SMAP is registered as the "sm0" device to the TCP/IP stack.
  if (ps2ip_getconfig((char*)"sm0", &ip_info) >= 0) {
    // Obtain the current DNS server settings.
    dns_curr = dns_getserver(0);

    ip_address[0] = ip4_addr1(reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr));
    ip_address[1] = ip4_addr2(reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr));
    ip_address[2] = ip4_addr3(reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr));
    ip_address[3] = ip4_addr4(reinterpret_cast<ip4_addr_t*>(&ip_info.ipaddr));

    netmask[0] = ip4_addr1(reinterpret_cast<ip4_addr_t*>(&ip_info.netmask));
    netmask[1] = ip4_addr2(reinterpret_cast<ip4_addr_t*>(&ip_info.netmask));
    netmask[2] = ip4_addr3(reinterpret_cast<ip4_addr_t*>(&ip_info.netmask));
    netmask[3] = ip4_addr4(reinterpret_cast<ip4_addr_t*>(&ip_info.netmask));

    gateway[0] = ip4_addr1(reinterpret_cast<ip4_addr_t*>(&ip_info.gw));
    gateway[1] = ip4_addr2(reinterpret_cast<ip4_addr_t*>(&ip_info.gw));
    gateway[2] = ip4_addr3(reinterpret_cast<ip4_addr_t*>(&ip_info.gw));
    gateway[3] = ip4_addr4(reinterpret_cast<ip4_addr_t*>(&ip_info.gw));

    dns[0] = ip4_addr1(dns_curr);
    dns[1] = ip4_addr2(dns_curr);
    dns[2] = ip4_addr3(dns_curr);
    dns[3] = ip4_addr4(dns_curr);

    TYRA_LOG(
        "IP:\t%d.%d.%d.%d\n"
        "NM:\t%d.%d.%d.%d\n"
        "GW:\t%d.%d.%d.%d\n"
        "DNS:\t%d.%d.%d.%d\n",
        ip_address[0], ip_address[1], ip_address[2], ip_address[3], netmask[0],
        netmask[1], netmask[2], netmask[3], gateway[0], gateway[1], gateway[2],
        gateway[3], dns[0], dns[1], dns[2], dns[3]);
    TYRA_LOG(ipAddress, "%d.%d.%d.%d", ip_address[0], ip_address[1],
             ip_address[2], ip_address[3]);
  } else {
    TYRA_ERROR("Unable to read IP address.\n");
  }
}

int TCP::DHCPCheck() {
  int EthernetLinkMode = NETMAN_NETIF_ETH_LINK_MODE_AUTO;
  int result;
  result = 0;
  do {
    // Wait for the link to become ready before changing the setting.
    if (ethWaitValidNetIFLinkState() != 0) {
      TYRA_ERROR("Error: failed to get valid link status.\n");
      result = 1;
      goto end;
    }

    // Attempt to apply the new link setting.
  } while (SetNetIFMode(EthernetLinkMode) != 0);

  if (ethWaitValidNetIFLinkState() != 0) {
    TYRA_ERROR("Failed to get valid link status.\n");
    result = 2;
    goto end;
  }

  TYRA_LOG("Waiting for DHCP lease...");
  // Wait for DHCP to initialize, if DHCP is enabled.
  if (ethWaitValidDHCPState() != 0) {
    TYRA_ERROR("DHCP failed\n.");
    result = 3;
    goto end;
  }
  TYRA_LOG("Initialized:\n");
  PrintIPConfig();

end:
  return result;
}

int TCP::End() {
  TYRA_LOG("Started TCP End. \n");
  ps2ip_deinit();
  NetManDeinit();
  return 0;
}

}  // namespace Tyra
