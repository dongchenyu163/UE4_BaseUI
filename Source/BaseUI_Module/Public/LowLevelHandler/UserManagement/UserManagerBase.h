// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInfoSaveGameObj.h"
#include "UserManagementGlobals.h"
#include "LowLevelHandler/LowLevelHandlerBase/LowLevelFunctionHandlerBase.h"
#include "Interfaces/I_UserManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "UserManagerBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BASEUI_MODULE_API UUserManagerBase : public ULowLevelFunctionHandlerBase, public II_UserManager
{
	GENERATED_BODY()

protected:
	virtual ~UUserManagerBase() override {}

#pragma region 从ULowLevelFunctionHandlerBase继承
public:
	virtual FName GetHandlerFName() override { return FName("UserManager"); }
	virtual TArray<FName> GetDependenceHandlerFNameList() override { return TArray<FName>(); }
	virtual EFunctionHandlerType GetHandlerType() override { return EFunctionHandlerType::LowLevelHandler; }
	virtual void InitHandler(II_GI_MenuFramework* InGameInstancePtr) override;
protected:
	virtual void OnStart() override {}
#pragma endregion 从ULowLevelFunctionHandlerBase继承
	
public:
	virtual ECreateUserFailed CreateNewUser_ByUserInfo_CPP(FUserInfo& InNewUserInfo) override;
	virtual ECreateUserFailed CreateNewUser_ByName_CPP(FName InUserName, const FText& InDisplayName, UTexture2D* InUserAvatar) override;
	virtual bool RemoveUser_ByName_CPP(FName InUserName) override;
	virtual bool RemoveUser_ByID_CPP(int64 UserID) override;
	virtual bool EditUser_ByName_CPP(FName InUserName, FUserInfo& InNewUserInfo) override;
	virtual bool EditUser_ByID_CPP(int64 UserID, FUserInfo& InNewUserInfo) override;
	virtual bool FindUser_ByName_CPP(FName InUserName, FUserInfo& OutFoundUserInfo) override;
	virtual bool FindUser_ByID_CPP(int64 UserID, FUserInfo& OutFoundUserInfo) override;
	virtual TArray<FUserInfo> GetAllUserInfo_CPP() override;
	virtual void SwitchCurrentUser_CPP(FName InUserNameToSwitch) override;
	virtual FUserInfo* GetCurrentUser_CPP() override { return &CurrentUser; }

	virtual ECreateUserFailed CreateNewUser_ByUserInfo_Implementation(FUserInfo& InNewUserInfo) override { return CreateNewUser_ByUserInfo_CPP(InNewUserInfo); }
	virtual ECreateUserFailed CreateNewUser_ByName_Implementation(FName InUserName, const FText& InDisplayName, UTexture2D* InUserAvatar) override { return CreateNewUser_ByName_CPP(InUserName, InDisplayName, InUserAvatar); }
	virtual bool RemoveUser_ByName_Implementation(FName InUserName) override { return RemoveUser_ByName_CPP(InUserName); }
	virtual bool RemoveUser_ByID_Implementation(int64 UserID) override { return RemoveUser_ByID_CPP(UserID); }
	virtual bool EditUser_ByName_Implementation(FName InUserName, FUserInfo& InNewUserInfo) override { return EditUser_ByName_CPP(InUserName, InNewUserInfo); }
	virtual bool EditUser_ByID_Implementation(int64 UserID, FUserInfo& InNewUserInfo) override { return EditUser_ByID_CPP(UserID, InNewUserInfo); }
	virtual bool FindUser_ByName_Implementation(FName InUserName, FUserInfo& OutFoundUserInfo) override { return FindUser_ByName_CPP(InUserName, OutFoundUserInfo); }
	virtual bool FindUser_ByID_Implementation(int64 UserID, FUserInfo& OutFoundUserInfo) override { return FindUser_ByID_CPP(UserID, OutFoundUserInfo); }
	virtual TArray<FUserInfo> GetAllUserInfo_Implementation() override { return (GetAllUserInfo_CPP()); }
	virtual void SwitchCurrentUser_Implementation(FName InUserNameToSwitch) override { SwitchCurrentUser_CPP(InUserNameToSwitch); }
	virtual FUserInfo GetCurrentUser_Implementation() override { return *GetCurrentUser_CPP(); }

	virtual int GetUserSaveIndex_ByID_CPP(int64 UserID) override;
	virtual int GetUserSaveIndex_ByID_Implementation(int64 UserID) override { return GetUserSaveIndex_ByID_CPP(UserID); }

	virtual FOnUserChanged* GetOnUserChangedDelegate_CPP() override { return &OnUserChanged; }
	
	void Init();
	/**
	 * @brief 初始化创建User，以及初始切换到合适的User上。
	 */
	void Init_User();
	void SaveUserInfo() const { UGameplayStatics::SaveGameToSlot(OperatingSaveGameObj, UUserManagerConfig::GetSaveSlotName(), UserInfoSaveIndex); }
	void LoadUserInfo();

protected:
	virtual int64 GetValidNewID(int64 InInitialID = INDEX_NONE);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Account|Save", meta=(Tooltip="一般不用动，存储所有用户信息的UserIndex，参考UGameplayStatics::SaveGameToSlot的同名参数"))
	int UserInfoSaveIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Account|Save", meta=(Tooltip=""))
	bool bIsInitial;


protected:
	UPROPERTY()
	UUserInfoSaveGameObj* OperatingSaveGameObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="")
	FUserInfo CurrentUser;

	UPROPERTY(BlueprintReadOnly, Category="", meta=(ToolTip=""))
	FOnUserChanged OnUserChanged;


};
