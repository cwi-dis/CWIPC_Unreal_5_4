// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CwipcSource.h"
#include "CwipcSourceSynthetic.h"
#include "CwipcSourceCamera.h"
#include "CwipcSourceFiles.h"
#include "CwipcSourceFactory.generated.h"

/**
 * 
 */
UCLASS()
class CWIPOINTCLOUDSUITEEDITOR_API UCwipcSourceFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	
	virtual bool ShouldShowInNewMenu() const override;

	virtual uint32 GetMenuCategories() const override;

	virtual FText GetDisplayName() const override;
};

UCLASS()
class CWIPOINTCLOUDSUITEEDITOR_API UCwipcSourceSyntheticFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ShouldShowInNewMenu() const override;

	virtual uint32 GetMenuCategories() const override;

	virtual FText GetDisplayName() const override;
};

UCLASS()
class CWIPOINTCLOUDSUITEEDITOR_API UCwipcSourceCameraFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ShouldShowInNewMenu() const override;

	virtual uint32 GetMenuCategories() const override;

	virtual FText GetDisplayName() const override;
};

UCLASS()
class CWIPOINTCLOUDSUITEEDITOR_API UCwipcSourceFilesFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ShouldShowInNewMenu() const override;

	virtual uint32 GetMenuCategories() const override;

	virtual FText GetDisplayName() const override;
};