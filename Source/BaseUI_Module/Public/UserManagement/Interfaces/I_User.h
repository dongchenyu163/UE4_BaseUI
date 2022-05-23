// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_User.generated.h"


USTRUCT(BlueprintType)
struct FUserBaseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Base Info", meta=(Tooltip="登录用户名（唯一）"))
	FName UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Base Info", meta=(Tooltip="用户的UID（唯一）"))
	int64 UserID = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Base Info", meta=(Tooltip="用户昵称"))
	FText UserDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Base Info", meta=(Tooltip="用户头像"))
	UTexture2D* UserAvatar;
};

USTRUCT(BlueprintType, meta=(ToolTip="用户的不好分类的杂项信息"))
struct FUserMiscInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Misc Info", meta=(Tooltip="初始化时默认切换到这个用户，多个用户都勾选的话，以第一个出现的用户为准"))
	bool bIsInitialSwitchToMe = false;
};

USTRUCT(BlueprintType, meta=(ToolTip="和用户相关的存档的信息；当前存档个数等信息"))
struct FUserSaveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|User Save Data", meta=(Tooltip=""))
	int MaxAutoSaveSlotSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|User Save Data", meta=(Tooltip=""))
	int MaxQuickSaveSlotSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|User Save Data", meta=(Tooltip=""))
	int AutoSaveHeaderSlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|User Save Data", meta=(Tooltip=""))
	int QuickSaveHeaderSlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account|User Save Data", meta=(Tooltip="已有存档的序号；存档Slot的命名：【存档Slot名称（每个用户不同）】+【存档序号】"))
	TArray<int32> SaveSlotList;
};

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

public:
	FUserInfo() { BaseInfo = FUserBaseInfo(); }
	FUserInfo(FName InUserName, FText InDisplayName, UTexture2D* InUserAvatar)
	{
		BaseInfo.UserAvatar = InUserAvatar;
		BaseInfo.UserName = InUserName;
		BaseInfo.UserDisplayName = InDisplayName;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account", meta=(Tooltip="用户基本信息"))
	FUserBaseInfo BaseInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account", meta=(Tooltip="存储一个用户的存档相关信息（不是游戏的存档）"))
	FUserSaveInfo SaveInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Account", meta=(Tooltip="不好分类的杂项信息"))
	FUserMiscInfo MiscInfo;

};

// This class does not need to be modified.
UINTERFACE()
class UI_User : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_User
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
