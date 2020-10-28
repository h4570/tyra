/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_STRING_
#define _TYRA_STRING_

#include <tamtypes.h>

class String
{

public:
    static char *createCopy(char *source);
    static u32 getLength(char *a);
    static char *createConcatenated(char *a, char *b);
    static char *createWithoutExtension(char *source);
};

#endif
