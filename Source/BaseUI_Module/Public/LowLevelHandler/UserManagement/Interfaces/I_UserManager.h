// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "I_User.h"
#include "UObject/Interface.h"
#include "I_UserManager.generated.h"


DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnUserChanged, const FUserInfo&, InOldUser, const FUserInfo&, InNewUser);

UENUM(BlueprintType)
enum ECreateUserFailed
{
	Success,
	DuplicatedID,
	NameHasIllegalChar
};

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UI_UserManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UserManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	ECreateUserFailed CreateNewUser_ByUserInfo(UPARAM(ref) FUserInfo& InNewUserInfo);
	virtual ECreateUserFailed CreateNewUser_ByUserInfo_CPP(FUserInfo& InNewUserInfo) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	ECreateUserFailed CreateNewUser_ByName(FName InUserName, const FText& InDisplayName, UTexture2D* InUserAvatar);
	virtual ECreateUserFailed CreateNewUser_ByName_CPP(FName InUserName, const FText& InDisplayName, UTexture2D* InUserAvatar) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool RemoveUser_ByName(FName InUserName);
	virtual bool RemoveUser_ByName_CPP(FName InUserName) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool RemoveUser_ByID(int64 UserID);
	virtual bool RemoveUser_ByID_CPP(int64 UserID) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool EditUser_ByName(FName InUserName, UPARAM(ref) FUserInfo& InNewUserInfo);
	virtual bool EditUser_ByName_CPP(FName InUserName, UPARAM(ref) FUserInfo& InNewUserInfo) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool EditUser_ByID(int64 UserID, UPARAM(ref) FUserInfo& InNewUserInfo);
	virtual bool EditUser_ByID_CPP(int64 UserID, UPARAM(ref) FUserInfo& InNewUserInfo) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool FindUser_ByName(FName InUserName, FUserInfo& OutFoundUserInfo);
	virtual bool FindUser_ByName_CPP(FName InUserName, FUserInfo& OutFoundUserInfo) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	bool FindUser_ByID(int64 UserID, FUserInfo& OutFoundUserInfo);
	virtual bool FindUser_ByID_CPP(int64 UserID, FUserInfo& OutFoundUserInfo) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	int GetUserSaveIndex_ByID(int64 UserID);
	virtual int GetUserSaveIndex_ByID_CPP(int64 UserID) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	TArray<FUserInfo> GetAllUserInfo();
	virtual TArray<FUserInfo> GetAllUserInfo_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	void SwitchCurrentUser(FName InUserNameToSwitch);
	virtual void SwitchCurrentUser_CPP(FName InUserNameToSwitch) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	FUserInfo GetCurrentUser();
	virtual FUserInfo* GetCurrentUser_CPP() = 0;

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="User Manager", meta=(ToolTip=""))
	// FOnUserChanged* GetCurrentUser();
	virtual FOnUserChanged* GetOnUserChangedDelegate_CPP() = 0;

};
