// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_UserManagement/UserManagementBaseHandler.h"
#include "LowLevelHandler/UserManagement/UserManagerBase.h"

const TMap<FString, UClass*> UUserManagementBaseHandler::Map_Purpose_To_DependenceHandlerClass = {
	TPair<FString, UClass*>("UserManager", UUserManagerBase::StaticClass()),
};
const TMap<FString, FText> UUserManagementBaseHandler::Map_Purpose_To_PurposeTooltip = {
	TPair<FString, FText>("UserManager", NSLOCTEXT("UUserManagementBaseHandler", "UserManager_Tooltip", "本依赖Handler的UI层级包装。")),
};
const FFunctionHandlerDef UUserManagementBaseHandler::HandlerDef(UUserManagementBaseHandler::StaticClass(), {
	HandlerDependentPair("UserManager", new FFunctionHandlerDependent(UUserManagerBase::StaticClass(),
		NSLOCTEXT("USavingBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。")))
});

ECreateUserFailed UUserManagementBaseHandler::CreateNewUser_CPP(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon)
{
	auto MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	auto ManagerPtr = MainMenuGameInstance->GetUserManager_CPP();

	auto CreateRes = ManagerPtr->CreateNewUser_ByName(InUserName, InUserDisplayName, InUserIcon);
	if (CreateRes == ECreateUserFailed::Success)
	{
		ManagerPtr->SwitchCurrentUser(InUserName);
	}
	return CreateRes;
}

void UUserManagementBaseHandler::GetUserList_CPP(TArray<FUserInfo>& OutUserList)
{
	auto MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	auto ManagerPtr = MainMenuGameInstance->GetUserManager_CPP();
	OutUserList = ManagerPtr->GetAllUserInfo_CPP();
}

FUserInfo* UUserManagementBaseHandler::GetCurrentUser_CPP()
{
	auto MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	auto ManagerPtr = MainMenuGameInstance->GetUserManager_CPP();
	return ManagerPtr->GetCurrentUser_CPP();
}
