/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#include "video/mpeg.hpp"
#include "utils/string.hpp"
#include "utils/debug.hpp"
#include "modules/audio.hpp"
#include <stdio.h>

FILE *mpg;


//Constructor
MPEG::MPEG() 
{
   
}

//Deconstructor
MPEG::~MPEG() 
{

}


//Initialize video
void MPEG::Video_Initialize(char *t_path)
{
  if (videoLoaded && audioLoaded)
  {
    MPEGSequenceInfo *m_Width;
    MPEGSequenceInfo *m_Height;
    MPEGSequenceInfo *Video;
    char *fullFilename = String::createConcatenated("host:", t_path);
    videoLoaded = true;
    
    if (MPEG_VIDEO_FORMAT_NTSC == 640 > 448)
    {
      mpg = fopen(fullFilename, "rb");
      m_Width->m_Height;
      consoleLog("Loaded video");
    }

    else if (MPEG_VIDEO_FORMAT_PAL == 512 > 448)
    {
      mpg = fopen(fullFilename, "rb");
      m_Width->m_Height;
      consoleLog("Loaded video");
    }

    else
    {
      assertMsg(mpg != NULL, "Failed to open mpg file");
    } 
  }
}

void MPEG::set_video_volume(const u8 &t_vol)
{
  u8 audsrv_volume;
  audsrv_volume = t_vol;
  audsrv_set_volume(audsrv_volume);
};

//mpeg suspend
void MPEG::Video_Suspend()
{
  MPEG::videoLoaded = false;
  fclose(mpg);
}

//resume video
void MPEG::Video_Resume()
{
   MPEG::resume = true;
}

//restart
void MPEG::Video_Reset()
{
   MPEG::reset = true;
}
