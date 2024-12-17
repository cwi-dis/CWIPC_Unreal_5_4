// Copyright Epic Games, Inc. All Rights Reserved.

#include "CWIPointCloudSuiteEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "CwipcSourceActions.h"

IMPLEMENT_GAME_MODULE(FCWIPointCloudSuiteEditorModule, CWIPointCloudSuiteEditor);

DEFINE_LOG_CATEGORY(CWIPointCloudSuiteEditor)

#define LOCTEXT_NAMESPACE "CWIPointCloudSuiteEditor"

void FCWIPointCloudSuiteEditorModule::StartupModule()
{
    CwipcSourceActions = MakeShared<FCwipcSourceActions>();
    FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(CwipcSourceActions.ToSharedRef());
#if 0
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    auto GameAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Cwipc")), LOCTEXT("CwipcCategory", "Cwipc"));
    AssetTools.RegisterAssetTypeActions(MakeShareable(new FCwipcSourceActions(GameAssetCategory)));
#endif
    UE_LOG(CWIPointCloudSuiteEditor, Warning, TEXT("CWIPointCloudSuiteEditor: Log Started"));
}

void FCWIPointCloudSuiteEditorModule::ShutdownModule()
{
    if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
    FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(CwipcSourceActions.ToSharedRef());
    UE_LOG(CWIPointCloudSuiteEditor, Warning, TEXT("CWIPointCloudSuiteEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE