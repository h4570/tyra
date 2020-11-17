/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_VU1_
#define _TYRA_VU1_

#include "../models/math/matrix.hpp"
#include <draw_buffers.h>
#include <tamtypes.h>
#include <draw_primitives.h>
#include <spacket.h>
#include <packet.h>
#include <math3d.h>

const u32 VIF_BUFFER_SIZE = 48 * 1024;

typedef struct VU1BuildList
{
    u8 isBuilding;
    void *offset;
    void *kickBuffer;
    u32 dmaSize;
    u32 dmaSizeAll;
    u32 dmaDestination;
} VU1BuildList;

/** Class responsible for managing VU1 micro programs */
class VU1
{

public:
    VU1();
    ~VU1();

    static void uploadProgram(int t_dest, u32 *t_start, u32 *t_end);
    void createList();
    void sendSingleRefList(int t_destAddress, void *t_data, int t_quadSize);
    void addListBeginning();
    void continueList();
    void addReferenceList(u32 t_offset, void *t_data, u32 t_size, u8 t_useTops);
    void addFloat(float v);
    void add32(u32 v);
    void add64(u64 v);
    void add128(u64 v1, u64 v2);
    void addListEnding();
    void addStartProgram();
    void addContinueProgram();
    void sendList();
    void addDoubleBufferSetting();
    void addFlush();

private:
    u8 isDoubleBufferSet;
    void checkDataAlignment(void *data);
    VU1BuildList buildList;
    spacket_t *spackets[2] __attribute__((aligned(16)));
    spacket_t *currPacket;
    u8 switchPacket;
    static u32 countProgramSize(u32 *t_start, u32 *t_end);
    void checkList();
    inline s32 AddUnpack(int format, int addr, int num, int usetops = 0, int nosign = 1, int masking = 0)
    {
        return (s32)((0x60 << 24) + (format << 24) + (masking << 28) + (usetops << 15) +
                     (nosign << 14) + (num << 16) + addr);
    }
};

#define DMA_REF_TAG(ADDR, COUNT) ((((unsigned long)ADDR) << 32) | (0x3 << 28) | COUNT)
#define DMA_CNT_TAG(COUNT) (((unsigned long)(0x1) << 28) | COUNT)
#define DMA_END_TAG(COUNT) (((unsigned long)(0x7) << 28) | COUNT)
#define VIF_NOP 0x00
#define VIF_STCYL 0x01
#define VIF_OFFSET 0x02
#define VIF_BASE 0x03
#define VIF_FLUSH 0x11
#define VIF_MSCAL 0x14
#define VIF_MSCNT 0x17
#define VIF_MPG 0x4A
#define V4_32 0xC
#define VIF_UNPACK 0x60
#define U128(n) ((u128)(n))
#define VIF_UNPACK_V4_32 (VIF_UNPACK | V4_32)
#define VIF_CODE(CMD, NUM, IMMEDIATE) ((((unsigned int)(CMD)) << 24) | \
                                       (((unsigned int)(NUM)) << 16) | \
                                       ((unsigned int)(IMMEDIATE)))
#define GS_PRIM(PRIM, IIP, TME, FGE, ABE, AA1, FST, CTXT, FIX) U128((FIX << 10) | (CTXT << 9) | (FST << 8) | (AA1 << 7) | (ABE << 6) | (FGE << 5) | (TME << 4) | (IIP << 3) | (PRIM))
#define GS_GIFTAG(NLOOP, EOP, PRE, PRIM, FLG, NREG) (((u64)(NREG) << 60) | ((u64)(FLG) << 58) | ((u64)(PRIM) << 47) | ((u64)(PRE) << 46) | (EOP << 15) | (NLOOP << 0))
#define VU1_DMA_CHAN_TIMEOUT -1
#define GS_GIFTAG_PACKED 0

#endif
