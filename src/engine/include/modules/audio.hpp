/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_AUDIO_
#define _TYRA_AUDIO_

#include <tamtypes.h>
#include <stdio.h>
#include <audsrv.h>
#include <kernel.h>
#include "../models/audio_listener.hpp"

#define STACK_SIZE 8 * 1024

/** Class responsible for audio playing */
class Audio
{

public:
    Audio();
    ~Audio();

    u8 isTrackDone;
    ee_sema_t sema;
    int fillbufferSema;

    void init(u32 t_listenersAmount);
    void play();
    void stop();
    void loadSong(char *t_filename);
    void unloadSong();
    void setVolume(u8 t_volume);
    void addListener(AudioListener *t_listener);
    void startThread();

private:
    void work();
    static void audioThread();
    u8 isInitialized, shouldPlay, songLoaded, isVolumeSet;

    ee_thread_t audioThreadAttr;
    u8 audioThreadStack[STACK_SIZE] ALIGNED(16);
    int audioThreadId;

    AudioListener **listeners;
    u32 listenersAmount;
    u32 addedListeners;
    void initSema();
    void loadModules();
    void initAUDSRV();

    int ret;
    char chunk[2048];
    FILE *wav;
    audsrv_fmt_t format;
    static int fillbuffer(void *arg);
};

#endif
