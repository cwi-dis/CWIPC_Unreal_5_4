// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterface.h"
#include "CwipcSource.h"
#include "VectorVM.h"
#include "CwipcNiagaraDataInterface.generated.h"


class cwipc_source;
class cwipc;
struct cwipc_point;


/**
 *
 */

UCLASS(EditInlineNew, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Niagara Data Interface"))
class CWIPOINTCLOUDSUITE_API UCwipcNiagaraDataInterface : public UNiagaraDataInterface
{
	GENERATED_UCLASS_BODY()

	BEGIN_SHADER_PARAMETER_STRUCT(FShaderParameters, )
		SHADER_PARAMETER(int32, PointsCount)
		SHADER_PARAMETER_SRV(Buffer<float4>, PositionsBuffer)
		SHADER_PARAMETER_SRV(Buffer<float4>, ColorBuffer)
	END_SHADER_PARAMETER_STRUCT()


	FORCEINLINE int32 GetNumberOfPoints()const { return CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 0; }

	// GPU compatibility 

	static const FString PointsCountParamName;
	static const FString PositionsBufferParamName;
	static const FString ColorsBufferParamName;


protected:
	virtual bool CopyToInternal(UNiagaraDataInterface* Destination) const override;
	bool warnedAboutLockBeforeInitialize = false;

public:

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Source Asset"))
	TObjectPtr<UCwipcSource> CwipcPointCloudSourceAsset;
	//----------------------------------------------------------------------------
	// UObject Interface
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual bool CanExecuteOnTarget(ENiagaraSimTarget Target) const override { return Target==ENiagaraSimTarget::GPUComputeSim; }
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//----------------------------------------------------------------------------
	// UNiagaraDataInterface Interface

	// Returns the DI's functions signatures 
	virtual void GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions)override;

	// Returns the delegate for the passed function signature.
	virtual void GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc) override;

	virtual bool Equals(const UNiagaraDataInterface* Other) const override;

	//----------------------------------------------------------------------------
	// EXPOSED FUNCTIONS
	// Initializes the source. Call once, at the start of the Niagara system or Niagara emitter.
	void InitializeSource(FVectorVMExternalFunctionContext& Context);

	// Get the next point cloud, if available, and lock it.  Call during Emitter Update.
	void LockPointCloud(FVectorVMExternalFunctionContext& Context);

	// Returns the relative timestamp of the current point cloud. Call during Emitter Update, after LockPointCloud.
	void GetTimeStamp(FVectorVMExternalFunctionContext& Context);

	// Returns the number of points in the current point cloud. Call during Emitter Update, after LockPointCloud.
	void GetNumberOfPoints(FVectorVMExternalFunctionContext& Context);

	// Returns the particle size for points in the current point cloud. Call during Emitter Update, after LockPointCloud.
	void GetParticleSize(FVectorVMExternalFunctionContext& Context);

	// Return the colors of a number of points.
	void GetColor(FVectorVMExternalFunctionContext& Context);

	// Return the positions of a number of points.
	void GetPosition(FVectorVMExternalFunctionContext& Context);

	// HLSL definitions for GPU 
	virtual void GetParameterDefinitionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, FString& OutHLSL) override;

	virtual bool GetFunctionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, const FNiagaraDataInterfaceGeneratedFunction& FunctionInfo,int32 FunctionINstanceIndex, FString& OutHLSL) override;
	
	//virtual bool UsesLegacyShaderBindings() const override;

	virtual void BuildShaderParameters(FNiagaraShaderParametersBuilder& ShaderParametersBuilder) const override;

	virtual void SetShaderParameters(const FNiagaraDataInterfaceSetShaderParametersContext& Context) const override;

};
