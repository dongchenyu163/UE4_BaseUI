// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/Examples/TestUI/TestUserGlobalSaveGameBaseObj.h"

#include "GUI_FrameWork/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "Kismet/GameplayStatics.h"

void UTestUserGlobalSaveGameBaseObj::DoUpdateUserGlobalData_Implementation()
{
	auto GI = GetFrameworkGameInstance_CPP();
	auto Info = GI->GetUIRuntimeInfoTMap_CPP();
	UserMapsInfo = *Info;
}
