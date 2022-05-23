// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "Interfaces/I_UI_UserManagement.h"
#include "UObject/Object.h"
#include "UserManagementBaseHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta=(ToolTip=""))
class BASEUI_MODULE_API UUserManagementBaseHandler : public UUIHandlerBase, public II_UI_UserManagement
{
	GENERATED_BODY()

protected:
	virtual ~UUserManagementBaseHandler() override {}
public:
	virtual ECreateUserFailed CreateNewUser_CPP(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon) override;
	virtual void GetUserList_CPP(TArray<FUserInfo>& OutUserList) override;
	virtual FUserInfo* GetCurrentUser_CPP() override;

	//      ECreateUserFailed CreateNewUser               (FName InUserName, FText InUserDisplayName, UTexture2D* InUserIcon)
	virtual ECreateUserFailed CreateNewUser_Implementation(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon) override { return CreateNewUser_CPP(InUserName, InUserDisplayName, InUserIcon); }
	virtual void GetUserList_Implementation(TArray<FUserInfo>& OutUserList) override { GetUserList_CPP(OutUserList); }
	virtual FUserInfo GetCurrentUser_Implementation() override { return *(GetCurrentUser_CPP()); }
};
