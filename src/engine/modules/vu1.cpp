/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/vu1.hpp"

#include "../include/utils/debug.hpp"
#include <dma.h>
#include <string.h>
#include <kernel.h>

// ----
// Constructors/Destructors
// ----

VU1::VU1()
{
    PRINT_LOG("Initializing VU1");
    currentBuffer = 0;
    switchBuffer = 0;
    PRINT_LOG("VU1 initialized!");
}

VU1::~VU1() {}

// ----
// Methods
// -

/** Create dynamic list */
void VU1::createList()
{
    switchBuffer = !switchBuffer;
    memset((char *)&buildList, 0, sizeof(buildList));

    if (switchBuffer)
        currentBuffer = (char *)&dmaBuffer1;
    else
        currentBuffer = (char *)&dmaBuffer2;
    buildList.kickBuffer = currentBuffer;
}

/** Add reference list and send via VIF1 
 * Not using TOPS register
 * Similar to addReferenceList() 
 */
void VU1::sendSingleRefList(int t_destAddress, void *t_data, int t_quadSize)
{
    checkDataAlignment(t_data);

    u8 tempBuffer[32] __attribute__((aligned(16)));
    void *chain = (u64 *)&tempBuffer; // uncached

    *((u64 *)chain)++ = DMA_REF_TAG((u32)t_data, t_quadSize);
    *((u32 *)chain)++ = VIF_CODE(VIF_STCYL, 0, 0x0101);
    *((u32 *)chain)++ = VIF_CODE(VIF_UNPACK_V4_32, t_quadSize, t_destAddress);

    *((u64 *)chain)++ = DMA_END_TAG(0);
    *((u32 *)chain)++ = VIF_CODE(VIF_NOP, 0, 0);
    *((u32 *)chain)++ = VIF_CODE(VIF_NOP, 0, 0);

    FlushCache(0);
    dma_channel_send_chain(DMA_CHANNEL_VIF1, tempBuffer, 0, DMA_FLAG_TRANSFERTAG, 0);
    dma_channel_wait(DMA_CHANNEL_VIF1, VU1_DMA_CHAN_TIMEOUT);
}

/** Add list beginning, set's double buffer if not set  */
void VU1::addListBeginning()
{
    if (buildList.isBuilding == 1)
        PRINT_ERR("Please end current list list before adding new one!");

    if (isDoubleBufferSet == 0)
        addDoubleBufferSetting();
    else
        addFlush();

    buildList.dmaSizeAll += buildList.dmaSize;
    buildList.dmaSize = 0;
    buildList.offset = currentBuffer;
    *((u64 *)currentBuffer)++ = DMA_CNT_TAG(0);                   // placeholder
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_STCYL, 0, 0x0101);   // placeholder
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32, 0, 0); // placeholder
    buildList.isBuilding = 1;
}

/** Add list ending and fix unpack size */
void VU1::addListEnding()
{
    if (buildList.isBuilding == 0)
    {
        PRINT_ERR("Please add list beginning first. Nothing to end!");
        return;
    }

    while ((buildList.dmaSize & 0xF))
    {
        *((u32 *)currentBuffer)++ = 0;
        buildList.dmaSize += 4;
    }

    *((u64 *)buildList.offset)++ = DMA_CNT_TAG(buildList.dmaSize >> 4);
    *((u32 *)buildList.offset)++ = VIF_CODE(VIF_STCYL, 0, 0x0101);
    *((u32 *)buildList.offset)++ = AddUnpack(V4_32, 0, buildList.dmaSize >> 4, 1);

    buildList.isBuilding = 0;
}

/** Add list which will load data from given pointer
 * A lot faster than standard list.
 * @param offset offset before data in quadwords
 * @param data data pointer
 * @param size in quadwords
 * @param useTops when true, data will be loaded at the beginning of buffer (BASE+OFFSET)
 */
void VU1::addReferenceList(u32 t_offset, void *t_data, u32 t_size, u8 t_useTops)
{
    checkDataAlignment(t_data);
    if (buildList.isBuilding == 1)
        PRINT_ERR("Please end current list list before adding new one!");
    *((u64 *)currentBuffer)++ = DMA_REF_TAG((u32)t_data, t_size);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_STCYL, 0, 0x0101);
    *((u32 *)currentBuffer)++ =
        AddUnpack(V4_32, t_useTops == 1 ? buildList.dmaSize >> 4 : t_offset >> 4, t_size, t_useTops);
    buildList.dmaSize += t_size * 8;
    buildList.dmaSizeAll += buildList.dmaSize;
}

/** Start VU1 program */
void VU1::addStartProgram()
{
    *((u64 *)currentBuffer)++ = DMA_CNT_TAG(0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_MSCAL, 0, 0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_FLUSH, 0, 0);
    ;
}

/** Continue VU1 program from "--cont" line */
void VU1::addContinueProgram()
{
    *((u64 *)currentBuffer)++ = DMA_CNT_TAG(0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_MSCAL, 0, 0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_FLUSH, 0, 0);
    ;
}

/** Add end tag and send packet via VIF1 */
void VU1::sendList()
{
    *((u64 *)currentBuffer)++ = DMA_END_TAG(0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_NOP, 0, 0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_NOP, 0, 0);
    FlushCache(0);
    dma_channel_send_chain(DMA_CHANNEL_VIF1, buildList.kickBuffer, (u32 *)currentBuffer - (u32 *)buildList.kickBuffer, DMA_FLAG_TRANSFERTAG, 0);
    dma_channel_wait(DMA_CHANNEL_VIF1, VU1_DMA_CHAN_TIMEOUT);
}

void VU1::addDoubleBufferSetting()
{
    *((u64 *)currentBuffer)++ = DMA_CNT_TAG(8 >> 4);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_BASE, 0, 8);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_OFFSET, 0, 496);
    isDoubleBufferSet = 1;
}

void VU1::addFlush()
{
    *((u64 *)currentBuffer)++ = DMA_CNT_TAG(8 >> 4);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_NOP, 0, 0);
    *((u32 *)currentBuffer)++ = VIF_CODE(VIF_FLUSH, 0, 0);
}

void VU1::checkDataAlignment(void *t_data)
{
    if (((u32)t_data & 0xF))
        PRINT_ERR("data is not 16 byte aligned!");
}

// ----
// List adding
// ----

void VU1::add128(u64 v1, u64 v2)
{
    checkList();
    *((u64 *)currentBuffer)++ = v1;
    *((u64 *)currentBuffer)++ = v2;
    buildList.dmaSize += 16;
}

void VU1::add64(u64 v)
{
    checkList();
    *((u64 *)currentBuffer)++ = v;
    buildList.dmaSize += 8;
}

void VU1::add32(u32 v)
{
    checkList();
    *((u32 *)currentBuffer)++ = v;
    buildList.dmaSize += 4;
}

void VU1::addFloat(float v)
{
    checkList();
    *((float *)currentBuffer)++ = v;
    buildList.dmaSize += 4;
}

void VU1::checkList()
{
    if (buildList.isBuilding == 0)
        PRINT_ERR("Please add list beginning before adding data!");
    if (buildList.dmaSizeAll > VIF_BUFFER_SIZE)
        PRINT_ERR("Buffer size exceed!");
}

// ----
// Static
// ----

u8 IS_DMA_VIF1_INITIALIZED = 0;
/** TODO */
void VU1::uploadProgram(int t_dest, u32 *t_start, u32 *t_end)
{
    if (!IS_DMA_VIF1_INITIALIZED)
    {
        IS_DMA_VIF1_INITIALIZED = 1;
        dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
        dma_channel_fast_waits(DMA_CHANNEL_VIF1);
    }

    int count = 0;
    u8 tempBuffer[512] __attribute__((aligned(16)));
    void *chain = (u64 *)&tempBuffer; // uncached

    // get the size of the code as we can only send 256 instructions in each MPGtag
    count = VU1::countProgramSize(t_start, t_end);
    while (count > 0)
    {
        u32 currentCount = count > 256 ? 256 : count;

        *((u64 *)chain)++ = DMA_REF_TAG((u32)t_start, currentCount / 2);
        *((u32 *)chain)++ = VIF_CODE(VIF_NOP, 0, 0);
        *((u32 *)chain)++ = VIF_CODE(VIF_MPG, currentCount & 0xFF, t_dest);

        t_start += currentCount * 2;
        count -= currentCount;
        t_dest += currentCount;
    }

    *((u64 *)chain)++ = DMA_END_TAG(0);
    *((u32 *)chain)++ = VIF_CODE(VIF_NOP, 0, 0);
    *((u32 *)chain)++ = VIF_CODE(VIF_NOP, 0, 0);

    // Send it to vif1
    FlushCache(0);
    dma_channel_send_chain(DMA_CHANNEL_VIF1, tempBuffer, 0, DMA_FLAG_TRANSFERTAG, 0);
    dma_channel_wait(DMA_CHANNEL_VIF1, VU1_DMA_CHAN_TIMEOUT); // synchronize immediately.
}

u32 VU1::countProgramSize(u32 *t_start, u32 *t_end)
{
    u32 size = (t_end - t_start) / 2;
    if (size & 1)
        size++;
    return size;
}
