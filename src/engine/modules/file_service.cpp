/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/file_service.hpp"
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

FileService *fsRef;

FileService::FileService() {}

FileService::~FileService() {}

// ----
// Methods
// ----

u32 FileService::addReadChunk(FILE *t_file, void *t_destination, const u32 &t_size, const u32 &t_n)
{
    FileServiceTask task;
    task.id = rand() % 1000000;
    task.file = t_file;
    task.destination = t_destination;
    task.size = t_size;
    task.n = t_n;
    task.readStatus = -2137;
    tasks.push_back(task);
    return task.id;
}

s32 FileService::isTaskDone(const u32 &t_taskId)
{
    s32 taskIndex = getIndexOf(t_taskId);
    assertMsg(taskIndex != -1, "Task was not found!");
    s32 result = tasks[taskIndex].readStatus;
    if (result != -2137)
        removeByIndex(taskIndex);
    return result;
}

const void FileService::removeById(const u32 &t_taskId)
{
    s32 index = getIndexOf(t_taskId);
    assertMsg(index != -1, "Cant remove task, because it was not found!");
    removeByIndex(index);
}

const s32 FileService::getIndexOf(const u32 &t_taskId)
{
    for (u32 i = 0; i < tasks.size(); i++)
        if (tasks[i].id == t_taskId)
            return i;
    return -1;
};

// Other

void FileService::startThread()
{
    consoleLog("Creating file service thread");
    extern void *_gp;
    thread.func = (void *)FileService::mainThread;
    thread.stack = threadStack;
    thread.stack_size = getThreadStackSize();
    thread.gp_reg = (void *)&_gp;
    thread.initial_priority = 0x12;
    threadId = CreateThread(&thread);
    assertMsg(threadId >= 0, "Create audio thread failed!");
    consoleLog("File service created");
    StartThread(threadId, NULL);
    consoleLog("File service started");
}

/** Main thread loop */
void FileService::threadLoop()
{
    // This will cause an error
    // printf("\n\n\n\n\nsize:%d\n", tasks.size());
    // for (size_t i = 0; i < tasks.size(); i++)
    // {
    //     switch (tasks[i].type)
    //     {
    //     case ReadChunk:
    //         tasks[i].readStatus = fread(tasks[i].destination, tasks[i].n, tasks[i].size, tasks[i].file);
    //         break;

    //     default:
    //         break;
    //     }
    // }
    SleepThread();
}

/** 
 * Do not call this function.
 * This is an file service thread, runned by engine
 */
void FileService::mainThread()
{
    while (true)
        fsRef->threadLoop();
}
