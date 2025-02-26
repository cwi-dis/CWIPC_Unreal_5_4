#pragma once

#include "CoreMinimal.h"
#include "cwipc_util/api.h"
#include "Containers/CircularQueue.h"

#include "CwipcSource.generated.h"

class UCwipcSource;
class cwipc_source;
class cwipc;
struct cwipc_point;

class FCwipcReaderThread : public FRunnable
{
public:
	FCwipcReaderThread(cwipc_source* _source, TCircularQueue<cwipc*>& _queue);
	virtual void StartThread();
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
protected:
	FRunnableThread* Thread;
	cwipc_source* source;
	TCircularQueue<cwipc*>& queue;
	bool bShutdown = false;
};
/**
 *
 */
UCLASS(BlueprintType, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Source"))
class CWIPOINTCLOUDSUITE_API UCwipcSource : public UObject
{
	friend class UCwipcNiagaraDataInterface;

	GENERATED_UCLASS_BODY()

protected:
	FCwipcReaderThread* readerThread;
	TCircularQueue<cwipc*> readerQueue = TCircularQueue<cwipc*>(4);
	FCriticalSection thread_lock;



	// Override this in a subclass if your subclass reads point clouds from a cwipc_source.
	virtual cwipc_source* _AllocateSource();
	// Alternatively, override this if your subclass does more in its thread, like reading files or running decompressors.
	virtual bool _AllocateReaderThread();

	/// <summary>
	/// Get a new point cloud into pc, if one is available.
	/// </summary>
	/// <returns>Returns true if a new point cloud was gotten.</returns>
	bool _CheckForNewPointCloudAvailable();

	/// <summary>
	/// Return true if we have a current point cloud. Does not get a new one.
	/// </summary>
	bool _ValidPointCloudAvailable();

	/// <summary>
	/// De-allocates everything in the right order.
	/// </summary>
	void _CleanupEverything();

public:

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "Cwipc Niagara", meta = (DisplayName = "Initialize source"))
	bool InitializeSource();
	cwipc* CheckForNewPointCloudAvailable();


	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Point size to use when unknown"))
	float defaultCellSize = 0.001;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Factor to convert point size to particle size"))
	float particle_size_factor = 1.0;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Synthetic generator: wanted point count"))
	int synthetic_wanted_pointcount = 400;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Synthetic generator: wanted clouds per second"))
	int synthetic_wanted_fps = 5;

	// Sample indexes for each point

};