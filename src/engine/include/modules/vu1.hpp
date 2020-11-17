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

#define V4_32 0xC
#define GS_PRIM(PRIM, IIP, TME, FGE, ABE, AA1, FST, CTXT, FIX) (u128)(((FIX << 10) | (CTXT << 9) | (FST << 8) | (AA1 << 7) | (ABE << 6) | (FGE << 5) | (TME << 4) | (IIP << 3) | (PRIM)))
#define GS_GIFTAG(NLOOP, EOP, PRE, PRIM, FLG, NREG) (((u64)(NREG) << 60) | ((u64)(FLG) << 58) | ((u64)(PRIM) << 47) | ((u64)(PRE) << 46) | (EOP << 15) | (NLOOP << 0))

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
    void addReferenceList(void *t_data, u32 t_size, u8 t_useTops);
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

    spacket_t *currPacket;

private:
    u32 qwordsInsertedToVU1;
    u8 isDoubleBufferSet, isBuilding;
    spacket_t *spackets[2] __attribute__((aligned(16)));
    u8 switchPacket;
    static u32 countProgramSize(u32 *t_start, u32 *t_end);
};

#endif
