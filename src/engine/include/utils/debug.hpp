/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_DEBUG_
#define _TYRA_DEBUG_

#ifdef NDEBUG
#define consoleLog(message) ((void)0)
#define assertMsg(condition, message) ((void)0)

#else // IF Debug
#include <stdio.h>
class Debug
{
public:
    static void trap(const char *t_text, const char *t_file)
    {
        printf("\n");
        printf("====================================\n");
        printf("| ERROR: %s\n", t_text);
        printf("| File : %s\n", t_file);
        printf("====================================\n\n");
        for (;;)
            ;
    }
};
#define consoleLog(message) printf("LOG: " message " (" __FILE__ ")\n")
#define assertMsg(condition, message) \
    if (!(condition))                 \
    Debug::trap(message, __FILE__)
#endif // NDEBUG

#endif // _TYRA_DEBUG_
