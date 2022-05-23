// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/I_User.h"
#include "UObject/Object.h"
#include "UserInfoSaveGameObj.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BASEUI_MODULE_API UUserInfoSaveGameObj : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|UserInfo", meta=(Tooltip=""))
	TMap<int64, FUserInfo> Map_UserID_To_UserInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|UserInfo", meta=(Tooltip=""))
	TMap<FName, int64> Map_UserName_To_UserID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|UserInfo", meta=(Tooltip=""))
	TArray<FName> UserNameList;

};
