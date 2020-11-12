/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_FILE_SERVICE_
#define _TYRA_FILE_SERVICE_

#include <tamtypes.h>
#include <stdio.h>
#include <kernel.h>
#include <vector>

enum FileServiceTaskType
{
    ReadChunk = 0
};

struct FileServiceTask
{
    FileServiceTaskType type;

    u32 id;

    /** 
     * Used by types:
     * - ReadChunk
     */
    FILE *file;

    /** 
     * Used by types:
     * - ReadChunk
     */
    s32 readStatus;

    /** 
     * Used by types:
     * - ReadChunk
     */
    void *destination;

    /** 
     * Used by types:
     * - ReadChunk
     */
    u32 size;

    /** 
     * Used by types:
     * - ReadChunk
     */
    u32 n;
};

/** Class responsible for background, non-blocking file loading playing. */
class FileService
{

public:
    /** Constructor is called by the engine. */
    FileService();
    ~FileService();

    /** 
     * Add task to read next chunk. ( fread() ) 
     * @returns Task id
     */
    u32 addReadChunk(FILE *t_file, void *t_destination, const u32 &t_size, const u32 &t_n);

    /** 
     * Check if given task was done. 
     * When task is done, this method will return read status. 
     * After this, task is removed from file service, so this method 
     * will return something once per task.
     * @returns Read status (ex. result of fread()). -2137 if task is not done.
     */
    s32 isTaskDone(const u32 &t_taskId);

    /** 
     * Start file service thread.
     * Do not call this method unless you know what you do.
     * Should be called by engine. 
     */
    void startThread();

    std::vector<FileServiceTask> tasks;

private:
    /** Remove task by index */
    void removeByIndex(const u32 &t_index) { tasks.erase(tasks.begin() + t_index); }

    /** Remove task by id */
    const void removeById(const u32 &t_taskId);

    /** 
     * Get index of task.
     * @returns Index of task. -1 if not found.
     */
    const s32 getIndexOf(const u32 &t_taskId);

    u8 threadStack[8 * 1024] __attribute__((aligned(16)));
    u32 getThreadStackSize() { return 8 * 1024; }
    ee_thread_t thread;
    int threadId;
    void threadLoop();
    static void mainThread();
};

#endif
