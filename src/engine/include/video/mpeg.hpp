/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
# Based on Eugene Plotnikov work: SIMPLE MEDIA SYSTEM
*/

#define __libmpeg_HPP
#define _MPEG_PT_I 1
#define _MPEG_PT_P 2
#define _MPEG_PT_B 3
#define _MPEG_PT_D 4

#define _MPEG_PS_TOP_FIELD    1
#define _MPEG_PS_BOTTOM_FIELD 2
#define _MPEG_PS_FRAME        3

#define _MPEG_MBT_INTRA           1
#define _MPEG_MBT_PATTERN         2
#define _MPEG_MBT_MOTION_BACKWARD 4
#define _MPEG_MBT_MOTION_FORWARD  8
#define _MPEG_MBT_QUANT          16

#define _MPEG_MC_FIELD 1
#define _MPEG_MC_FRAME 2
#define _MPEG_MC_16X8  2
#define _MPEG_MC_DMV   3

#define _MPEG_MV_FIELD 0
#define _MPEG_MV_FRAME 1

#define _MPEG_CODE_PIC_START 0x00000100
#define _MPEG_CODE_SLICE_MIN 0x00000101
#define _MPEG_CODE_SLICE_MAX 0x000001AF
#define _MPEG_CODE_USER_DATA 0x000001B2
#define _MPEG_CODE_SEQ_HDR   0x000001B3
#define _MPEG_CODE_EXTENSION 0x000001B5
#define _MPEG_CODE_SEQ_END   0x000001B7
#define _MPEG_CODE_GRP_START 0x000001B8

#define _MPEG_XID_0         0
#define _MPEG_XID_SEQUENCE  1
#define _MPEG_XID_DISPLAY   2
#define _MPEG_XID_QMATRIX   3
#define _MPEG_XID_COPYRIGHT 4
#define _MPEG_XID_SCALABLE  5
#define _MPEG_XID_6         6
#define _MPEG_XID_PIC_DSP   7
#define _MPEG_XID_PIC_COD   8
#define _MPEG_XID_PIC_SSC   9
#define _MPEG_XID_PIC_TSC  10

#define MPEG_CHROMA_FORMAT_420 1
#define MPEG_CHROMA_FORMAT_422 2
#define MPEG_CHROMA_FORMAT_444 3

#define MPEG_PROFILE_422        133
#define MPEG_PROFILE_SIMPLE       5
#define MPEG_PROFILE_MAIN_        4
#define MPEG_PROFILE_SNR_SCALABLE 3
#define MPEG_PROFILE_SPT_SCALABLE 2
#define MPEG_PROFILE_HIGH         1

#define MPEG_LEVEL_MAIN      8
#define MPEG_LEVEL_LOW      12
#define MPEG_LEVEL_HIGH1440  6
#define MPEG_LEVEL_HIGH      4

#define MPEG_VIDEO_FORMAT_COMPONENT 0
#define MPEG_VIDEO_FORMAT_PAL 1
#define MPEG_VIDEO_FORMAT_NTSC 2
#define MPEG_VIDEO_FORMAT_SECAM 3
#define MPEG_VIDEO_FORMAT_MAC 4
#define MPEG_VIDEO_FORMAT_UNSPEC 5

#define MPEG_RESET_TIME    0x00000004
#define MPEG_RESET_RECOVER 0x00000008
typedef struct MPEGSequenceInfo 
{
 int m_Width;
 int m_Height;
 int m_FrameCnt;
 int m_Profile;
 int m_Level;
 int m_ChromaFmt;
 int m_VideoFmt;
 int m_fEOF;
 int m_MSPerFrame;
} MPEGSequenceInfo;

typedef struct _MPEGMBXY 
{
 unsigned char m_X;
 unsigned char m_Y;
} _MPEGMBXY;

typedef struct _MPEGMacroBlock8 
{
 unsigned char m_Y [16][16];
 unsigned char m_Cb[  8 ][  8 ];
 unsigned char m_Cr[  8 ][  8 ];
} _MPEGMacroBlock8;

typedef struct _MPEGMotion 
{
 unsigned char* m_pSrc;
 short*         m_pDstY;
 short*         m_pDstCbCr;
 int            m_X;
 int            m_Y;
 int            m_H;
 int            m_fInt;
 int            m_Field;
 void ( *MC_Luma   ) ( struct _MPEGMotion* );
 void ( *MC_Chroma ) ( void                );
} _MPEGMotion;

typedef struct _MPEGMotions 
{
 unsigned char* m_pMBDstY;
 unsigned char* m_pMBDstCbCr;
 unsigned char* m_pSrc;
 unsigned char* m_pSPRBlk;
 unsigned char* m_pSPRRes;
 unsigned char* m_pSPRMC;
 int            m_Stride;
 int            m_nMotions;
 void           ( *BlockOp ) ( struct _MPEGMotions* );
 _MPEGMotion    m_Motion[ 7 ];
} _MPEGMotions;

typedef struct _MPEGContext 
{
 MPEGSequenceInfo  m_SI;
 long              m_FwdPTS;
 long              m_BckPTS;
 long              m_AuxPTS;
 int               m_MBWidth;
 int               m_MBHeight;
 _MPEGMacroBlock8* m_pFwdFrame;
 _MPEGMacroBlock8* m_pBckFrame;
 _MPEGMacroBlock8* m_pAuxFrame;
 _MPEGMacroBlock8* m_pCurFrame;
 _MPEGMBXY*        m_pMBXY;
 unsigned char*    m_pCurFrameY;
 unsigned char*    m_pCurFrameCbCr;
 int               m_FRCode;
 int               m_MBStride;
 int               m_MBCount;
 int               m_fProgSeq;
 int               m_fMPEG2;
 int               m_fRepFF;
 int               m_fTopFF;
 int               m_fFPFrmDCT;
 int               m_fConsMV;
 int               m_fSecField;
 int               m_fError;
 int               m_fDCRst;
 int               m_fRecover;
 int               m_QScale;
 int               m_PictStruct;
 int               m_PictCodingType;
 int               m_FPFVector;
 int               m_FwdFCode;
 int               m_FPBVector;
 int               m_BckFCode;
 int               m_FCode[2][2];
 int               m_CurMC;
 _MPEGMotions      m_MC[2];
 _MPEGMotions*     m_pCurMotions;
} _MPEGContext;

class Video
{

 public:
  void MPEG_Initialize(char *path);
  void MPEG_Destroy();
  void MPEG_Suspend();
  void MPEG_Resume();
  void MPEG_Reset();
  int *MPEG_Picture();
  void MPEG_Flush();
  int MPEG_CSCImage();
  int MPEG_GetMBType();
  int MPEG_GetMotionCode();
  unsigned int MPEG_GetBits();
  unsigned int MPEG_ShowBits();
  void MPEG_set_volume(int volume);
  void MPEG_get_resolution(MPEGSequenceInfo *m_Width, MPEGSequenceInfo *m_Height);
private:
  int MPEG_GetMBAI();
  void MPEG_SetDefQM();
  void MPEG_SetQM();
  void MPEG_SetIDCP();
  void MPEG_SetQSTIVFAS();
  void MPEG_SetPCT();
  void MPEG_BDEC();
  int MPEG_WaitBDEC();
  void MPEG_dma_ref_image();
  void MPEG_do_mc();
  void MPEG_put_luma();
  void MPEG_put_luma_X();
  void MPEG_put_luma_Y();
  void MPEG_put_luma_XY();
  void MPEG_put_chroma();
  void MPEG_put_chroma_X();
  void MPEG_put_chroma_Y();
  void MPEG_put_chroma_XY();
  void MPEG_avg_luma();
  void MPEG_avg_luma_X();
  void MPEG_avg_luma_Y();
  void MPEG_avg_luma_XY();
  void MPEG_avg_chroma();
  void MPEG_avg_chroma_X();
  void MPEG_avg_chroma_Y();
  void MPEG_avg_chroma_XY();
  void MPEG_put_block_fr();
  void MPEG_put_block_fl();
  void MPEG_put_block_il();
  void MPEG_add_block_ilfl();
  void MPEG_add_block_frfr();
  void MPEG_add_block_frfl();
  void MPEG_Set16();
  int MPEG_GetDMVector();
  unsigned int MPEG_NextStartCode();
  void MPEG_AlignBits();
};

