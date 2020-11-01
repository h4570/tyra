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

#include <tamtypes.h>
#include <math3d.h>
#include <stdio.h>

class Debug
{
public:
    static void errTrap(char *t_text, char *t_file)
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

#define PRINT_LOG(TEXT) printf("LOG: " TEXT " (" __FILE__ ")\n")
#define PRINT_ERR(TEXT) Debug::errTrap(TEXT, __FILE__)

#endif
