/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/audio.hpp"

#include <loadfile.h>
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"

#define SONG_NAME "MOV-CIRC.WAV"
const int AUDSRV_BUFFER_SIZE = 1024 * 4;

// ----
// Constructors/Destructors
// ----
Audio *audioRef;

Audio::Audio()
{
    addedListeners = 0;
    isInitialized = 0;
    songLoaded = 0;
    isVolumeSet = 0;
    shouldPlay = 0;
    audioRef = this;
    initSema();
    loadModules();
    initAUDSRV();
}

Audio::~Audio() {}

// ----
// Methods
// ----

/** Initialize audio module
 * 
 * - Initialize threading semaphore
 * - Load modules
 * - Initialize AUDSRV
 * - Load background song
 */
void Audio::init(u32 t_listenersAmount)
{
    PRINT_LOG("Initialize audio module started");
    listenersAmount = t_listenersAmount;
    listeners = new AudioListener *[t_listenersAmount];
    isInitialized = true;
    PRINT_LOG("Audio module initialized!");
}

void Audio::addListener(AudioListener *t_listener)
{
    listeners[addedListeners++] = t_listener;
}

/** Initialize threading semaphore */
void Audio::initSema()
{
    PRINT_LOG("Creating semaphore started");
    sema.init_count = 0;
    sema.max_count = 1;
    sema.option = 0;
    fillbufferSema = CreateSema(&sema);
    PRINT_LOG("Semaphore created");
}

/** Load LIBSD and AUDSRV */
void Audio::loadModules()
{
    PRINT_LOG("Modules loading started (LIBSD, AUDSRV)");
    ret = SifLoadModule("rom0:LIBSD", 0, NULL);
    if (ret == -203)
        PRINT_ERR("LIBSD loading failed!");
    ret = SifLoadModule("host:AUDSRV.IRX", 0, NULL);
    if (ret == -203)
        PRINT_ERR("AUDSRV.IRX loading failed!");
    PRINT_LOG("Modules loaded");
}

/** Initialize AUDSRV and install fillbuffer callback */
void Audio::initAUDSRV()
{
    PRINT_LOG("Initialize AUDSRV started");
    ret = audsrv_init();
    if (ret != 0)
    {
        PRINT_ERR("Failed to initialize AUDSRV!");
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
    }
    else
    {
        ret = audsrv_on_fillbuf(AUDSRV_BUFFER_SIZE, Audio::fillbuffer, (void *)fillbufferSema);
        PRINT_LOG("AUDSRV initialized!");
    }
}

/** Set audio format, volume and load song */
void Audio::loadSong(char *t_filename)
{
    if (!isInitialized)
    {
        PRINT_ERR("Please initialize audio class first!");
        return;
    }
    PRINT_LOG("Song loading started");
    format.bits = 16;
    format.freq = 22050;
    format.channels = 2;
    char *fullFilename = String::createConcatenated("host:", t_filename);
    int err = audsrv_set_format(&format);
    if (err == 0)
    {
        PRINT_LOG("Audio format set");
        if (!isVolumeSet)
            setVolume(MAX_VOLUME);
        PRINT_LOG("Opening song file: " SONG_NAME);
        wav = fopen(fullFilename, "rb");
        delete[] fullFilename;
        if (wav == NULL)
        {
            PRINT_ERR("Failed to open wav file!");
            audsrv_quit();
        }
        else
        {
            fseek(wav, 0x30, SEEK_SET);
            played = 0;
            songLoaded = 1;
            PRINT_LOG("Song loaded!");
        }
    }
    else
    {
        PRINT_ERR("Failed to set audio format!");
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
    }
}

void Audio::setVolume(u8 t_volume)
{
    audsrv_set_volume(t_volume);
    isVolumeSet = true;
}

/** Create and start audio thread. */
void Audio::startThread()
{
    PRINT_LOG("Creating audio thread");
    extern void *_gp;
    audioThreadAttr.func = (void *)Audio::audioThread;
    audioThreadAttr.stack = audioThreadStack;
    audioThreadAttr.stack_size = STACK_SIZE;
    audioThreadAttr.gp_reg = (void *)&_gp;
    audioThreadAttr.initial_priority = 0x17;
    if ((audioThreadId = CreateThread(&audioThreadAttr)) < 0)
        PRINT_ERR("Create audio thread failed!");
    PRINT_LOG("Audio thread created");
    StartThread(audioThreadId, NULL);
    PRINT_LOG("Audio thread started");
}

/** Do not call this function.
 * This is a audio thread, runned by AudioThread::start()
*/
void Audio::audioThread()
{
    while (true)
    {
        if (audioRef->shouldPlay)
            audioRef->work();
    }
}

void Audio::play()
{
    if (!isInitialized)
    {
        PRINT_ERR("Please initialize audio class first!");
        return;
    }
    shouldPlay = 1;
}

void Audio::stop()
{
    if (!isInitialized)
    {
        PRINT_ERR("Please initialize audio class first!");
        return;
    }
    shouldPlay = 0;
}

/** Play song and run it again on finish */
void Audio::work()
{
    if (!isTrackDone)
    {
        ret = fread(chunk, 1, sizeof(chunk), wav);
        if (ret > 0)
        {
            WaitSema(fillbufferSema);
            audsrv_play_audio(chunk, ret);
        }
        if (ret < (int)sizeof(chunk))
        {
            isTrackDone = true;
            return;
        }
        played++;
        if ((played + 20) % 41 == 0 && addedListeners)
            for (u32 i = 0; i < addedListeners; i++)
                listeners[i]->onAudioTick();
    }
    else
    {
        PRINT_LOG("Song " SONG_NAME " finished. Running again...");
        played = 0;
        fseek(wav, 0x30, SEEK_SET);
        isTrackDone = false;
    }
}

/** Unload audio module by closing file stream and stopping AUDSRV */
void Audio::unloadSong()
{
    PRINT_LOG("Unloading audio module started");
    fclose(wav);
    PRINT_LOG("Song file stream closed");
    audsrv_quit();
    PRINT_LOG("AUDSRV stopped");
    PRINT_LOG("Audio module unloaded");
}

/** Do not call this function. 
 * This is a static callback function
 * for AUDSRV fillbuffer (semaphore)
 */
int Audio::fillbuffer(void *arg)
{
    iSignalSema((int)arg);
    return 0;
}
