// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetOperations.h"
#include "ContentBrowserModule.h"
#include "DataTableEditorUtils.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "IContentBrowserSingleton.h"
#include "MapDescriptor.h"
#include "MapInfo.generated.h"


USTRUCT(BlueprintType, meta=(ToolTip="描述游戏内某个World的结构体，该结构体应当创建DataTable来定义游戏中所有和UI相关的World。本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。"))
/**
 * 描述游戏内某个World的结构体，\n 每个游戏应当创建一个DataTable来定义游戏中所有和UI相关的World。\n
 * 本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。
 */
struct BASEUI_MODULE_API FMapInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="Map ID，地图的唯一识别名称，RowName应和此一致"))
	FName MapIdentifier = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="显示在【地图选择界面】界面中的名称"))
	FText MapDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="相关联的World资产"))
	TSoftObjectPtr<UWorld> MapObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="显示在【地图选择界面】界面中的图片，一般来讲是关卡缩略图"))
	UTexture2D* MapThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="对用户描述当前关卡的文本，比如【任务简报】"))
	FText MapDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="方便分类关卡。比如：有好几个【地图选择界面】，每个地图选择界面都只显示包含对应Tag的关卡，例如【常驻关卡】【困难关卡】【活动关卡】等"))
	FGameplayTagContainer MapTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Addition Info", meta=(Tooltip="可根据游戏自定义的额外信息，存储在DataAsset中"))
	UMapDescriptor* AdditionalInfoObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Addition Info", meta=(Tooltip="关卡完成后的UI的关联的【下一关】；或者当前关卡完成后可以开启的关卡列表（普通益智游戏的话应该只有1个元素；复杂的游戏可能会有多个）"))
	TArray<FName> NextLevel_MapIdentifierList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Playing Info", meta=(Tooltip="运行时判断某个World是否为进行游戏的世界（比如：显示开始菜单的World就不是）；默认为  是。用途：UI的显示，主界面的菜单可能不需要背景模糊功能，游玩过程中，暂停界面需要背景模糊。有了这个变量就可以区分。"))
	/**
	 * 运行时判断某个World是否为进行游戏的世界（比如：显示开始菜单的World就不是）；默认为  是。
	 * 用途：UI的显示，主界面的菜单可能不需要背景模糊功能，游玩过程中，暂停界面需要背景模糊。有了这个变量就可以区分。
	 */
	bool bIsGameWorld = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Playing Info", meta=(Tooltip="该地图在用户的初始进度下，是否可以游玩？ 默认不能。"))
	bool bInitialPlayable = false;

public:
	bool IsValid() const
	{
		return !MapObject.IsNull() && !MapIdentifier.IsNone();  // 地图不为空，且 地图的ID也不为空
	}

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		static bool bIsRecurrent = false;
		if (bIsRecurrent)
		{
			return;
		}

		FMapInfo* EditingRowPtr = InDataTable->FindRow<FMapInfo>(InRowName, TEXT(""));

		if (EditingRowPtr->MapIdentifier == NAME_None)
		{
			EditingRowPtr->MapIdentifier = InRowName;
		}
		else if (EditingRowPtr->MapIdentifier != InRowName)
		{
			const bool bNameNotExist = InDataTable->FindRow<FMapInfo>(EditingRowPtr->MapIdentifier, TEXT("")) == nullptr;
			if (bNameNotExist)
			{
				UDataTable* EditingDataTable = LoadObject<UDataTable>(nullptr, *InDataTable->GetPackage()->FileName.ToString());
				if (EditingDataTable == nullptr )
				{
					UAssetOperations::SimpleSaveAsset((UDataTable*)InDataTable);
				}
				bIsRecurrent = true;
				EditingDataTable->AddRow(EditingRowPtr->MapIdentifier, *EditingRowPtr);
				// auto res = (FMapInfo*)FDataTableEditorUtils::AddRow(EditingDataTable, EditingRowPtr->MapIdentifier);
				EditingDataTable->Modify();
				// *res = *EditingRowPtr;
				FDataTableEditorUtils::RemoveRow(EditingDataTable, InRowName);
				// EditingDataTable->RemoveRow(InRowName);
				FDataTableEditorUtils::SelectRow(EditingDataTable, EditingRowPtr->MapIdentifier);
				// FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
				// ContentBrowserModule.Get().SyncBrowserToAssets(TArray<UObject*>({ EditingDataTable }), true);
				// EditingDataTable->MarkPackageDirty();
				
				bIsRecurrent = false;
			}
		}
		
	}
};

USTRUCT(BlueprintType, meta=(ToolTip="描述游戏内某个World的结构体，该结构体应当创建DataTable来定义游戏中所有和UI相关的World。本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。"))
/**
 * 描述游戏内某个World的结构体，\n 每个游戏应当创建一个DataTable来定义游戏中所有和UI相关的World。\n
 * 本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。
 */
struct BASEUI_MODULE_API FMapInfo_User
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="Map ID，地图的唯一识别名称."))
	FName MapIdentifier = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="", meta=(Tooltip="当前关卡是否 【可以游玩？】或【已解锁？】"))
	bool bIsPlayable = false;

	FMapInfo_User() {}
	FMapInfo_User(FMapInfo& InMapInfo) :
	MapIdentifier(InMapInfo.MapIdentifier),
	bIsPlayable(InMapInfo.bInitialPlayable) {}
};


