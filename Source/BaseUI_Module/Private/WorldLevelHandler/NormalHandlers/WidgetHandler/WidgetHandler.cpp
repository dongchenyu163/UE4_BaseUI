// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/NormalHandlers/WidgetHandler/WidgetHandler.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "LowLevelHandler/SaveLoad/SavingBaseHandler.h"
#include "WorldLevelHandler/DA_WidgetInfo.h"

void UWidgetHandler::OnStart()
{
	Super::OnStart();
	ResetWidgetInfo();
}

void UWidgetHandler::ResetWidgetInfo()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.ClassNames.Add(*UDA_WidgetInfo::StaticClass()->GetName());
	Filter.PackagePaths.Add("/Game/");
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);
	for (FAssetData AssetData : AssetDataList)
	{
		auto WidgetInfoObj = Cast<UDA_WidgetInfo>(AssetData.GetAsset());
		WidgetInfoObj->WidgetRef = nullptr;
	}
}
