// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/MapInfo.h"
#include "UObject/Interface.h"
#include "I_UI_MapSelectable.generated.h"


USTRUCT(BlueprintType, meta=(ToolTip=""))
struct FMapUIRuntimeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="", meta=(Tooltip="（原则上，运行时不可更改）仅是为了唯一标识或者寻找FMapUIRuntimeInfo对象"))
	FName MapIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="", meta=(Tooltip="当前关卡是否 【可以游玩？】或【已解锁？】"))
	bool bIsPlayable;

};

USTRUCT(BlueprintType, meta=(ToolTip="描述游戏内某个World的结构体，该结构体应当创建DataTable来定义游戏中所有和UI相关的World。本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。"))
/**
 * 描述游戏内某个World的结构体，\n 每个游戏应当创建一个DataTable来定义游戏中所有和UI相关的World。\n
 * 本结构体多用于 方便程序在【地图选择界面】中显示游玩的关卡信息。
 */
struct FMapUIInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="Map ID，地图的唯一识别名称，RowName应和此一致"))
	FName MapIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="显示在【地图选择界面】界面中的名称"))
	FText MapDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="相关联的World资产"))
	TSoftObjectPtr<UWorld> MapObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="显示在【地图选择界面】界面中的图片，一般来讲是关卡缩略图"))
	UTexture2D* MapThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Addition Info", meta=(Tooltip="对用户描述当前关卡的文本，比如【任务简报】"))
	FText MapDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Addition Info", meta=(Tooltip="方便分类关卡。比如：有好几个【地图选择界面】，每个地图选择界面都只显示包含对应Tag的关卡，例如【常驻关卡】【困难关卡】【活动关卡】等"))
	FGameplayTagContainer MapTags;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Playing Info", meta=(Tooltip="是否在地图选择界面中显示，默认显示。【多人游戏大厅】【主菜单】所使用的World就不能出现在地图选择中"))
	bool bShowingAtMapSelection = true;

public:
	bool IsValid() const
	{
		return !MapObject.IsNull() && !MapIdentifier.IsNone();  // 地图不为空，且 地图的ID也不为空
	}
};


// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UI_UI_MapSelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_MapSelectable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MapSelection")
	TArray<FName> GetMapIDList(UDataTable* InMapInfoDataTable);
	virtual TArray<FName> GetMapIDList_CPP(UDataTable* InMapInfoDataTable) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MapSelection", meta=(ToolTip="蓝图用，获取MapUIInfo的一份拷贝，因为蓝图没法传递普通C++指针"))
	TArray<FMapInfo> GetMapInfoList(UDataTable* InMapInfoDataTable);
	// C++专用，可以直接读取DataTable的内存。
	virtual TArray<FMapInfo*> GetMapInfoList_CPP(UDataTable* InMapInfoDataTable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MapSelection")
	void LoadMap(FMapInfo InMapInfo);
	virtual void LoadMap_CPP(const FMapInfo* InMapInfo) = 0;

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MapSelection", meta=(Tooltip=""))
	// void LoadMap_ByID(FName InMapID);
	// virtual void LoadMap_ByID_CPP(FName InMapID) = 0;

};
