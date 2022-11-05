/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#include "thread/threading.hpp"
#include <time.h>

namespace Tyra {

timespec Threading::tv = {0, 0};

#if defined(XBOX_360) || defined(OG_XBOX)
bool Threading::nanosleep(const timespec *rqtp, timespec *rmtp)
{
	/* Declarations */
	HANDLE timer;	/* Timer handle */
	LARGE_INTEGER li;	/* Time defintion */

	if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;

	li.QuadPart = -100000000LL;

	if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, false)){
		CloseHandle(timer);
		return false;
	}

	/* Start & wait for timer */
	WaitForSingleObject(timer, INFINITE);
	/* Clean resources */
	CloseHandle(timer);
	/* Slept without problems */
return true;
}
#endif

#ifdef XBOX_360
void Threading::sleep(const uint32_t& ms) {
  tv.tv_sec = ms / 1000;
  tv.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&tv, nullptr);
}
#endif
void Threading::sleep(const timespec& t_tv) 
{ 
    nanosleep(&tv, NULL);
}

void Threading::switchThread() {
  tv.tv_sec = 0;
  tv.tv_nsec = 500000;  // 1/2ms
  nanosleep(&tv, nullptr);
  sleep(1000);
}

}  // Namespace Tyra