// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_UserManagement/UserManagementBaseHandler.h"
#include "LowLevelHandler/UserManagement/UserManagerBase.h"

const FFunctionHandlerDef UUserManagementBaseHandler::HandlerDef(UUserManagementBaseHandler::StaticClass(), {
	HandlerDependentPair("UserManager", new FFunctionHandlerDependent(UUserManagerBase::StaticClass(),
		NSLOCTEXT("USavingBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。")))
});

void UUserManagementBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	UserManagerPtr = dynamic_cast<II_UserManager*>(Map_Purpose_To_HandlerInstance["UserManager"]);
}

ECreateUserFailed UUserManagementBaseHandler::CreateNewUser_CPP(FName InUserName, const FText& InUserDisplayName, UTexture2D* InUserIcon)
{
	auto CreateRes = UserManagerPtr->CreateNewUser_ByName_CPP(InUserName, InUserDisplayName, InUserIcon);
	if (CreateRes == ECreateUserFailed::Success)
	{
		UserManagerPtr->SwitchCurrentUser_CPP(InUserName);
	}
	return CreateRes;
}

void UUserManagementBaseHandler::GetUserList_CPP(TArray<FUserInfo>& OutUserList)
{
	OutUserList = UserManagerPtr->GetAllUserInfo_CPP();
}

FUserInfo* UUserManagementBaseHandler::GetCurrentUser_CPP()
{
	return UserManagerPtr->GetCurrentUser_CPP();
}
