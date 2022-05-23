// Fill out your copyright notice in the Description page of Project Settings.


#include "LowLevelHandler/SaveLoad/ClassAndStructs/SaveGameObjBase.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "Kismet/GameplayStatics.h"

// void USaveGameObjBase::FillDataToSaveGameObject_Implementation()
// {
//
// }

II_GI_MenuFramework* USaveGameObjBase::GetFrameworkGameInstance_CPP()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(this);
	II_GI_MenuFramework* MainMenuGameInstance(Cast<II_GI_MenuFramework>(GameInstance));
	if (MainMenuGameInstance)
	{
		return MainMenuGameInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] GameInstance convert to II_GI_MenuFramework failed. Forgot changing the Game Instance?\
			 We need Game Instance that dirived from [II_GI_MenuFramework] interface."), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}
}