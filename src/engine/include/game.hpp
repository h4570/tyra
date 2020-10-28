/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_GAME_
#define _TYRA_GAME_

#include <tamtypes.h>

class Game
{

public:
    virtual ~Game(){};
    virtual void onInit() = 0;
    virtual void onUpdate() = 0;

private:
};

#endif
