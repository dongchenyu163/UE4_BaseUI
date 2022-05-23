// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_UserManagement/UserManagementBaseHandler.h"

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
