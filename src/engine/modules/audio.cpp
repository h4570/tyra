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
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include <loadfile.h>
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

Audio *audioRef;

Audio::Audio()
{
    // We must set it to 0 on songStop();
    realVolume = 100;
    volume = 100;
    chunkReadStatus = 0;

    songLoaded = false;
    songPlaying = false;
    songInLoop = false;
    songFinished = false;

    audioRef = this;
    initSema();
    loadModules();
    initAUDSRV();
    setSongFormat();
}

Audio::~Audio() {}

// ----
// Methods
// ----

void Audio::loadSong(char *t_path)
{
    if (songLoaded)
        unloadSong();
    char *fullFilename = String::createConcatenated("host:", t_path);
    wav = fopen(fullFilename, "rb");
    delete[] fullFilename;
    if (wav == NULL)
        PRINT_ERR("Failed to open wav file!");
    else
    {
        rewindSongToStart();
        songLoaded = true;
        PRINT_LOG("Song loaded!");
    }
}

void Audio::playSong()
{
    if (!songLoaded)
        PRINT_ERR("Cant play song because was not loaded!");
    if (songFinished)
        rewindSongToStart();
    volume = realVolume;
    songPlaying = true;
}

void Audio::stopSong()
{
    volume = 0;
    audsrv_set_volume(volume);
    songPlaying = false;
}

void Audio::setSongVolume(const u8 &t_vol)
{
    realVolume = t_vol;
    if (songPlaying)
        volume = t_vol;
    audsrv_set_volume(volume);
}

u32 Audio::addSongListener(AudioListener *t_listener)
{
    AudioListenerRef ref;
    ref.id = rand() % 1000000;
    ref.listener = t_listener;
    songListeners.push_back(ref);
    return ref.id;
}

void Audio::removeSongListener(const u32 &t_id)
{
    s32 index = -1;
    for (u32 i = 0; i < songListeners.size(); i++)
        if (songListeners[i].id == t_id)
        {
            index = i;
            break;
        }
    if (index == -1)
        PRINT_ERR("Cant remove listener because given id was not found!");
    songListeners.erase(songListeners.begin() + index);
}

// ADPCM

audsrv_adpcm_t *Audio::loadADPCM(char *t_path)
{
    char *fullFilename = String::createConcatenated("host:", t_path);
    FILE *file = fopen(fullFilename, "rb");
    delete[] fullFilename;
    fseek(file, 0, SEEK_END);
    u32 adpcmFileSize = ftell(file);
    u8 data[adpcmFileSize];
    rewind(file);
    fread(data, sizeof(u8), adpcmFileSize, file);
    audsrv_adpcm_t *result = new audsrv_adpcm_t();
    result->size = 0;
    result->buffer = 0;
    result->loop = 0;
    result->pitch = 0;
    result->channels = 0;
    if (audsrv_load_adpcm(result, data, adpcmFileSize))
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("audsrv_load_adpcm() failed!");
    }
    fclose(file);
    return result;
}

void Audio::playADPCM(audsrv_adpcm_t *t_adpcm)
{
    if (audsrv_play_adpcm(t_adpcm))
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("audsrv_play_adpcm() failed!");
    }
}

void Audio::playADPCM(audsrv_adpcm_t *t_adpcm, const s8 &t_ch)
{
    if (audsrv_ch_play_adpcm(t_ch, t_adpcm))
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("audsrv_play_adpcm() failed!");
    }
}

// Other

void Audio::startThread(FileService *t_fileService)
{
    PRINT_LOG("Creating audio thread");
    fileService = t_fileService;
    extern void *_gp;
    thread.func = (void *)Audio::mainThread;
    thread.stack = threadStack;
    thread.stack_size = getThreadStackSize();
    thread.gp_reg = (void *)&_gp;
    thread.initial_priority = 0x17;
    if ((threadId = CreateThread(&thread)) < 0)
        PRINT_ERR("Create audio thread failed!");
    PRINT_LOG("Audio thread created");
    StartThread(threadId, NULL);
    PRINT_LOG("Audio thread started");
}

/** Main thread loop */
void Audio::threadLoop()
{
    if (!songPlaying || !songLoaded)
        return;
    if (songFinished)
    {
        printf("Audio: Song finished. ");
        if (songInLoop)
        {
            printf("Running again.\n");
            for (u32 i = 0; i < getSongListenersCount(); i++)
                songListeners[i].listener->onAudioFinish();
            rewindSongToStart();
        }
        else
        {
            printf("Stopping song.\n");
            stopSong();
            return;
        }
    }

    if (chunkReadStatus > 0)
    {
        WaitSema(fillbufferSema); // wait until previous chunk wasn't finished
        audsrv_play_audio(wavChunk, chunkReadStatus);
        for (u32 i = 0; i < getSongListenersCount(); i++)
            songListeners[i].listener->onAudioTick();
    }

    chunkReadStatus = fread(wavChunk, 1, sizeof(wavChunk), wav);

    if (chunkReadStatus < (s32)sizeof(wavChunk))
        songFinished = true;
}

/** 
 * Close file.
 * Delete song path from memory.
 */
void Audio::unloadSong()
{
    songLoaded = false;
    fclose(wav);
}

/** Set WAV format to 16bit, 22050Hz, stereo. */
void Audio::setSongFormat()
{
    format.bits = 16;
    format.freq = 22050;
    format.channels = 2;
    audsrv_set_format(&format);
}

/** Fseek on wav. */
void Audio::rewindSongToStart()
{
    if (wav != NULL)
        fseek(wav, 0x30, SEEK_SET);
    songFinished = false;
}

/** Initialize semaphore which will wait until chunk of the song is not finished. */
void Audio::initSema()
{
    PRINT_LOG("Creating audio semaphore");
    sema.init_count = 0;
    sema.max_count = 1;
    sema.option = 0;
    fillbufferSema = CreateSema(&sema);
    PRINT_LOG("Audio semaphore created");
}

/** Load LIBSD and AUDSRV modules */
void Audio::loadModules()
{
    PRINT_LOG("Modules loading started (LIBSD, AUDSRV)");
    int ret = SifLoadModule("rom0:LIBSD", 0, NULL);
    if (ret == -203)
        PRINT_ERR("LIBSD loading failed!");
    ret = SifLoadModule("host:AUDSRV.IRX", 0, NULL);
    if (ret == -203)
        PRINT_ERR("AUDSRV.IRX loading failed!");
    PRINT_LOG("Audio modules loaded");
}

/** 
 * Initialize AUDSRV main and ADPCM module. 
 * Install AUDSRV callback.
 */
void Audio::initAUDSRV()
{
    PRINT_LOG("Initializing AUDSRV");
    int ret = audsrv_init();
    if (ret != 0)
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("Failed to initialize AUDSRV!");
    }
    ret = audsrv_adpcm_init();
    if (ret != 0)
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("Failed to initialize AUDSRV ADPCM!");
    }
    ret = audsrv_on_fillbuf(getSongBufferSize(), (audsrv_callback_t)iSignalSema, (void *)fillbufferSema);
    if (ret != 0)
    {
        printf("AUDSRV returned error string: %s", audsrv_get_error_string());
        PRINT_ERR("Failed to initialize AUDSRV fillbuffer!");
    }
    PRINT_LOG("AUDSRV initialized!");
}

/** 
 * Do not call this function.
 * This is a audio thread, runned by engine
 */
void Audio::mainThread()
{
    while (true)
        audioRef->threadLoop();
}
