// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "CwipcSourceActions.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(CWIPointCloudSuiteEditor, All, All);

class FCWIPointCloudSuiteEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	TSharedPtr<FCwipcSourceActions> CwipcSourceActions;
};
