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

#include "../models/audio_listener.hpp"
#include "./file_service.hpp"
#include <tamtypes.h>
#include <stdio.h>
#include <audsrv.h>
#include <kernel.h>
#include <vector>

struct AudioListenerRef
{
    AudioListener *listener;
    u32 id;
};

/** 
 * Class responsible for audio playing.
 * There are two main features:
 * - WAV song (background music)
 * - ADPCM samples
 */
class Audio
{

public:
    /** Constructor is called by the engine. */
    Audio();
    ~Audio();

    // Song

    /** 
     * Load 24bit 22050Hz Stereo WAV. 
     * Can be used multiple times for song switching. 
     * @param t_path Example: "song.wav" or "folder/song.wav"
     */
    void loadSong(char *t_path);

    void playSong();

    void stopSong();

    /** 
     * Set song volume.
     * @param t_vol Value 0-100
     */
    void setSongVolume(const u8 &t_vol);

    /** 
     * If you want to synchronize some actions with 
     * background song, add audio listener. 
     * @returns Listener id.
     */
    u32 addSongListener(AudioListener *t_listener);

    /** Remove song listener. */
    void removeSongListener(const u32 &t_id);

    const u8 &isSongPlaying() const { return songPlaying; }

    const u8 &isSongLoaded() const { return songLoaded; }

    const u8 &isSongInLoop() const { return songInLoop; }

    u32 getSongListenersCount() const { return songListeners.size(); }

    const u8 &getVolume() const { return volume; }

    /** 
     * Set song loop/noloop.
     * @param t_set false - noloop, true - loop
     */
    void setSongLoop(const u8 &t_set) { songInLoop = t_set; }

    // ADPCM

    /** 
     * Load ADPCM sample.
     * @param t_path Example: "hit.adpcm" or "folder/jump.adpcm"
     */
    audsrv_adpcm_t *loadADPCM(char *t_path);

    /** 
     * Play ADPCM sample.
     * ADPCM sample can't be stopped.
     * @param t_adpcm ADPCM data, created by loadADPCM();
     */
    void playADPCM(audsrv_adpcm_t *t_adpcm);

    /** 
     * Play ADPCM sample.
     * ADPCM sample can't be stopped.
     * @param t_adpcm ADPCM data, created by loadADPCM();
     * @param t_ch Channel (0-23). Type -1 for use any free channel. 
     */
    void playADPCM(audsrv_adpcm_t *t_adpcm, const s8 &t_ch);

    /** 
     * Set ADPCM volume.
     * @param t_vol Value 0-100
     * @param t_ch Channel (0-23)
     */
    void setADPCMVolume(u8 t_vol, const s8 &t_ch) { audsrv_adpcm_set_volume(t_ch, t_vol); }

    // Other

    /** 
     * Start audio thread.
     * Do not call this method unless you know what you do.
     * Should be called by engine. 
     */
    void startThread(FileService *t_fileService);

private:
    u8 songLoaded, volume, realVolume, songPlaying, songInLoop, songFinished;
    std::vector<AudioListenerRef> songListeners;
    FILE *wav;
    audsrv_fmt_t format;
    FileService *fileService;

    char wavChunk[2 * 1024] __attribute__((aligned(16)));
    s32 chunkReadStatus;

    u8 threadStack[8 * 1024] __attribute__((aligned(16)));
    u32 getThreadStackSize() { return 8 * 1024; }
    u32 getSongBufferSize() { return 4 * 1024; }
    ee_thread_t thread;
    int threadId;
    ee_sema_t sema;
    s32 fillbufferSema;

    void setSongFormat();
    void unloadSong();
    void rewindSongToStart();

    void initSema();
    void loadModules();
    void initAUDSRV();
    void threadLoop();
    static void mainThread();
};

#endif
