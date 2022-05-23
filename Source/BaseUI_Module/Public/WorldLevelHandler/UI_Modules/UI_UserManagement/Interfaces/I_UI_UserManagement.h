// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LowLevelHandler/UserManagement/Interfaces/I_User.h"
#include "LowLevelHandler/UserManagement/Interfaces/I_UserManager.h"
#include "I_UI_UserManagement.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UI_UI_UserManagement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_UserManagement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI User Manage", meta=(Tooltip=""))
	ECreateUserFailed CreateNewUser(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon);
	virtual ECreateUserFailed CreateNewUser_CPP(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI User Manage", meta=(Tooltip=""))
	void GetUserList(TArray<FUserInfo>& OutUserList);
	virtual void GetUserList_CPP(TArray<FUserInfo>& OutUserList) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI User Manage", meta=(Tooltip=""))
	FUserInfo GetCurrentUser();
	virtual FUserInfo* GetCurrentUser_CPP() = 0;

};
