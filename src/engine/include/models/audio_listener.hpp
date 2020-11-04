/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_AUDIO_LISTENER_
#define _TYRA_AUDIO_LISTENER_

class AudioListener
{

public:
    virtual ~AudioListener(){};
    virtual void onAudioTick() = 0;
    virtual void onAudioFinish() = 0;

private:
};

#endif
