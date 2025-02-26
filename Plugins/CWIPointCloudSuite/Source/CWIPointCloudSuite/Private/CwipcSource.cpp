// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"
#include <chrono>

// Define as empty to get debug prints
#define DBG
// Define is if(0) to not get debug prints
// #define DBG if(0)


UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer),
    readerThread(nullptr),
    readerQueue(20)

{
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::UCwipcSource() called"), *GetPathNameSafe(this));
}


void UCwipcSource::_CleanupEverything()
{
    // xxxjack _CleanupEverything should also be called during destroction, after editing a source, etc.
    if (readerThread) {
        readerThread->Stop();
        readerThread = nullptr;
    }
#ifdef xxxjack_moved_to_instance_data
    FScopeLock lock(&pc_lock);
    if (pc != nullptr) {
        pc->free();
        pc = nullptr;
    }
    if (pc_points != nullptr) {
        free(pc_points);
        pc_points = nullptr;
        pc_points_count = 0;
    }
#endif
}

void UCwipcSource::PostInitProperties()
{
    Super::PostInitProperties();
}

void UCwipcSource::PostLoad()
{
    Super::PostLoad();
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::PostLoad() called"), *GetPathNameSafe(this));
    _CleanupEverything();
}

void UCwipcSource::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource::PostEditChangeProperty() on 0x%p called"), (void*)this);
    _CleanupEverything();
}

void UCwipcSource::BeginDestroy()
{
    Super::BeginDestroy();
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::BeginDestroy() called"), *GetPathNameSafe(this));
    _CleanupEverything();
}

cwipc_source* UCwipcSource::_AllocateSource()
{
    char* errorMessage = nullptr;
    cwipc_source* source = cwipc_synthetic(synthetic_wanted_fps, synthetic_wanted_pointcount, &errorMessage, CWIPC_API_VERSION);
    if (source == nullptr)
    {
        if (errorMessage)
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned error: %s"), *GetPathNameSafe(this), errorMessage ? UTF8_TO_TCHAR(errorMessage) : TEXT("Unknown error"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned null, but no error message"), *GetPathNameSafe(this));
        }
    }
    return source;
}

bool UCwipcSource::_AllocateReaderThread()
{
    cwipc_source* source = _AllocateSource();
    if (source == nullptr)
    {
        // _AllocateSource will have given an error message
        return false;
    }
    readerThread = new FCwipcReaderThread(source, readerQueue);
    readerThread->StartThread();
    DBG UE_LOG(LogTemp, Display, TEXT("UcwipcSource[%s]: created point cloud source"), *GetPathNameSafe(this));
    return true;
}

bool UCwipcSource::InitializeSource()
{
    FScopeLock lock(&thread_lock);
    if (readerThread != nullptr) {
        return true;
    }

    return _AllocateReaderThread();
}


cwipc* UCwipcSource::CheckForNewPointCloudAvailable()
{
    if (readerThread == nullptr) {
        // xxxjack DBG UE_LOG(LogTemp, Warning, TEXT("UcwipcSource[%s]: _CheckForNewPointCloudAvailable: source == NULL, Initializing"), *GetPathNameSafe(this));
        // xxxjack InitializeSource();
        return nullptr;
    }

    if (readerQueue.IsEmpty())
    {
        return nullptr;
    }
    cwipc* new_pc = nullptr;
    if (!readerQueue.Dequeue(new_pc))
    {
        // Should not happen?
        return nullptr;
    }
    return new_pc;
}

FCwipcReaderThread::FCwipcReaderThread(cwipc_source* _source, TCircularQueue<cwipc*>& _queue)
    : source(_source),
    queue(_queue)
{
}

void FCwipcReaderThread::StartThread() {
    Thread = FRunnableThread::Create(this, TEXT("CwipcReaderThread"));
}

bool FCwipcReaderThread::Init()
{
    return true;
}

uint32 FCwipcReaderThread::Run()
{
    while (!bShutdown && source != nullptr) {
        if (source->eof()) {
            return 1;
        }
        if (source->available(true)) {
            cwipc* pc = source->get();
            if (pc == nullptr) {
                UE_LOG(LogTemp, Error, TEXT("FCwipcReaderThread::Run: get() returned NULL"));
                return 2;
            }
            if (!queue.Enqueue(pc)) {
                pc->free();
                UE_LOG(LogTemp, Warning, TEXT("FCwipcReaderThread::Run: dropped point cloud, queue full"));
            }
        }
    }
    return 0;
}

void FCwipcReaderThread::Exit()
{
    if (source) {
        source->free();
    }
}

void FCwipcReaderThread::Stop()
{
    bShutdown = true;
    cwipc* pc;
    while (queue.Dequeue(pc)) {
        pc->free();
    }
}
