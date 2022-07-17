/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/
#include <tamtypes.h>
#include <libmpeg.h>

class MPEG
{
public:

  MPEG();
  ~MPEG();
  
  u32 videoLoaded;
  u32 resume;
  u8 audioLoaded;
  u32 reset;
  /** 
  * Load Video here. 
  * @param t_path Example: "Sample.mpg" or "folder/Sample.mpg"
  */ 
  void Video_Initialize(char *t_path);
  void Video_Suspend();
  void Video_Resume();
  void Video_Reset();
  void set_video_volume(const u8 &t_vol);
  void Video_get_resolution(MPEGSequenceInfo *m_Width, MPEGSequenceInfo *m_Height);
};
