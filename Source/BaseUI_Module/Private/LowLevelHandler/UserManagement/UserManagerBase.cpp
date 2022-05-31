// Fill out your copyright notice in the Description page of Project Settings.


#include "LowLevelHandler/UserManagement/UserManagerBase.h"

#include "Kismet/GameplayStatics.h"
#include "LowLevelHandler/UserManagement/UserManagementGlobals.h"

void UUserManagerBase::InitHandler(II_GI_MenuFramework* InGameInstancePtr, TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
{
	Super::InitHandler(InGameInstancePtr, InDependencyHandlerDict);
	LoadUserInfo();
	bIsInitial = true;

	Init_User();
}

ECreateUserFailed UUserManagerBase::CreateNewUser_ByUserInfo_CPP(FUserInfo& InNewUserInfo)
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	TArray<FName>& UserNameList = OperatingSaveGameObj->UserNameList;
	
	InNewUserInfo.BaseInfo.UserID = GetValidNewID(InNewUserInfo.BaseInfo.UserID);
	UserInfoMap.Add(InNewUserInfo.BaseInfo.UserID, InNewUserInfo);

	// 拼接用户名和UID，防止用户名重复。
	// FName UID = FName(InNewUserInfo.BaseInfo.UserName.ToString() + FString::Printf(TEXT("%lld"),InNewUserInfo.BaseInfo.UserID));
	UserNameMap.Add(InNewUserInfo.BaseInfo.UserName, InNewUserInfo.BaseInfo.UserID);
	UserNameList.Add(InNewUserInfo.BaseInfo.UserName);

	SaveUserInfo();
	return ECreateUserFailed::Success;
}

ECreateUserFailed UUserManagerBase::CreateNewUser_ByName_CPP(FName InUserName, const FText& InDisplayName, UTexture2D* InUserAvatar)
{
	FUserInfo NewUserInfo(InUserName, InDisplayName, InUserAvatar);
	return CreateNewUser_ByUserInfo_CPP(NewUserInfo);
}

bool UUserManagerBase::RemoveUser_ByName_CPP(FName InUserName)
{
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	int64* FoundIndex = UserNameMap.Find(InUserName);
	if (FoundIndex)
	{
		return RemoveUser_ByID_CPP(*FoundIndex);
	}
	return false;
}

bool UUserManagerBase::RemoveUser_ByID_CPP(int64 UserID)
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	TArray<FName>& UserNameList = OperatingSaveGameObj->UserNameList;

	FUserInfo* FoundUserInfoPtr = UserInfoMap.Find(UserID);
	if (FoundUserInfoPtr)
	{
		UserNameMap.Remove(FoundUserInfoPtr->BaseInfo.UserName);
		UserNameList.Remove(FoundUserInfoPtr->BaseInfo.UserName);
		UserInfoMap.Remove(UserID);
		SaveUserInfo();
		return true;
	}
	return false;
}

bool UUserManagerBase::EditUser_ByName_CPP(FName InUserName, FUserInfo& InNewUserInfo)
{
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	int64* FoundIndex = UserNameMap.Find(InUserName);
	if (FoundIndex)
	{
		return EditUser_ByID_CPP(*FoundIndex, InNewUserInfo);
	}
	return false;
}

bool UUserManagerBase::EditUser_ByID_CPP(int64 UserID, FUserInfo& InNewUserInfo)
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	TArray<FName>& UserNameList = OperatingSaveGameObj->UserNameList;
	
	FUserInfo* FoundUserInfoPtr = UserInfoMap.Find(UserID);
	if (FoundUserInfoPtr)
	{
		UserNameMap.Add(InNewUserInfo.BaseInfo.UserName, UserNameMap[InNewUserInfo.BaseInfo.UserName]);
		UserNameMap.Remove(FoundUserInfoPtr->BaseInfo.UserName);

		UserNameList.Remove(FoundUserInfoPtr->BaseInfo.UserName);
		UserNameList.Add(InNewUserInfo.BaseInfo.UserName);

		FoundUserInfoPtr->BaseInfo.UserName = InNewUserInfo.BaseInfo.UserName;
		FoundUserInfoPtr->BaseInfo.UserDisplayName = InNewUserInfo.BaseInfo.UserDisplayName;
		FoundUserInfoPtr->BaseInfo.UserAvatar = InNewUserInfo.BaseInfo.UserAvatar;
		SaveUserInfo();
		return true;
	}
	return false;
}

bool UUserManagerBase::FindUser_ByName_CPP(FName InUserName, FUserInfo& OutFoundUserInfo)
{
	TMap<FName, int64>& UserNameMap = OperatingSaveGameObj->Map_UserName_To_UserID;
	int64* FoundIndex = UserNameMap.Find(InUserName);
	if (FoundIndex)
	{
		return FindUser_ByID_CPP(*FoundIndex, OutFoundUserInfo);
	}
	return false;
}

bool UUserManagerBase::FindUser_ByID_CPP(int64 UserID, FUserInfo& OutFoundUserInfo)
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;

	FUserInfo* FoundUserInfoPtr = UserInfoMap.Find(UserID);
	if (FoundUserInfoPtr)
	{
		OutFoundUserInfo = *FoundUserInfoPtr;
		return true;
	}
	return false;
}

TArray<FUserInfo> UUserManagerBase::GetAllUserInfo_CPP()
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;
	TArray<FUserInfo> Res;
	UserInfoMap.GenerateValueArray(Res);
	return Res;
}

void UUserManagerBase::SwitchCurrentUser_CPP(FName InUserNameToSwitch)
{
	FUserInfo UserInfo;
	if (FindUser_ByName_CPP(InUserNameToSwitch, UserInfo))
	{
		const auto OldUserInfo = CurrentUser;
		CurrentUser = UserInfo;
		const bool&& Res = OnUserChanged.ExecuteIfBound(OldUserInfo, UserInfo);
	}
}

int UUserManagerBase::GetUserSaveIndex_ByID_CPP(int64 UserID)
{
	static FUserInfo Res;
	if (FindUser_ByID_CPP(UserID, Res))
	{
		return OperatingSaveGameObj->UserNameList.Find(Res.BaseInfo.UserName);
	}
	else
	{
		return INDEX_NONE;
	}
}

void UUserManagerBase::Init()
{
	LoadUserInfo();
	bIsInitial = true;

	Init_User();
}

void UUserManagerBase::Init_User()
{
	const auto UserList = GetAllUserInfo_CPP();
	// 没有User且 设置中【Initial Auto Create User】被勾选。
	// 创建默认User
	if (UUserManagerConfig::IsInitialAutoCreateUser() && UserList.Num() == 0)
	{
		const FName NewUserName(UUserManagerConfig::GetDefaultUserName());
		CreateNewUser_ByName_CPP(NewUserName, FText::FromName(NewUserName), nullptr);
		const FUserInfo UserInfo = GetAllUserInfo_CPP()[0];
		SwitchCurrentUser_CPP(UserInfo.BaseInfo.UserName);
	}
	// 否则，
	// 有User，切换至合适的User
	// 没有一个User，什么也不做。
	else
	{
		if (UserList.Num() > 0)
		{
			int SwitchUserIndex = INDEX_NONE;
			for (int i = 0; i < UserList.Num(); i++)
			{
				const auto UserInfo = UserList[i];
				if (UserInfo.MiscInfo.bIsInitialSwitchToMe)
				{
					// 用户信息中，有用户bIsInitialSwitchToMe置为true了，初始默认切换到该用户
					SwitchUserIndex = i;
					SwitchCurrentUser_CPP(UserInfo.BaseInfo.UserName);
					break;
				}
			}

			// 所有用户的bIsInitialSwitchToMe都是false，初始默认切换到【第一个存储的用户】。
			if (SwitchUserIndex == INDEX_NONE)
			{
				const FUserInfo UserInfo = GetAllUserInfo_CPP()[0];
				SwitchCurrentUser_CPP(UserInfo.BaseInfo.UserName);
			}
		}
	}
}

void UUserManagerBase::LoadUserInfo()
{
	bool bHasValidUserInfo = false;
	if (UGameplayStatics::DoesSaveGameExist(UUserManagerConfig::GetSaveSlotName(), UserInfoSaveIndex))
	{
		USaveGame* LoadedSaveGameObj = UGameplayStatics::LoadGameFromSlot(UUserManagerConfig::GetSaveSlotName(), UserInfoSaveIndex);
		UUserInfoSaveGameObj* UserSaveGameInfo = Cast<UUserInfoSaveGameObj>(LoadedSaveGameObj);
		if (UserSaveGameInfo)
		{
			bHasValidUserInfo = true;
			OperatingSaveGameObj = UserSaveGameInfo;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Function:[%s] Loaded [USaveGame] Object is NOT a [UUserInfoSaveGameObj] Object!!"), ANSI_TO_TCHAR(__FUNCTION__));
			bHasValidUserInfo = false;
		}
	}
	
	if (bHasValidUserInfo == false)
	{
		OperatingSaveGameObj = NewObject<UUserInfoSaveGameObj>(this);
		SaveUserInfo();
	}
}

int64 UUserManagerBase::GetValidNewID(int64 InInitialID)
{
	TMap<int64, FUserInfo>& UserInfoMap = OperatingSaveGameObj->Map_UserID_To_UserInfo;
	int64 ID = INDEX_NONE;
	if (InInitialID == INDEX_NONE)
	{
		ID = UUserManagerConfig::GetStartUserID() + UserInfoMap.Num();
	}
	else
	{
		ID = InInitialID;
	}
	bool bIsDuplicatedID = UserInfoMap.Find(ID) != nullptr;
	while (bIsDuplicatedID)
	{
		ID++;
		bIsDuplicatedID = UserInfoMap.Find(ID) != nullptr;
	}
	return ID;
}
