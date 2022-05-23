// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"

#include "Kismet/GameplayStatics.h"

// TScriptInterface<II_GI_MenuFramework> UUIHandlerBase::GetFrameworkGameInstance()
// {
// 	// auto GameInstance = UGameplayStatics::GetGameInstance(this);
// 	// II_GI_MenuFramework* MainMenuGameInstance(Cast<II_GI_MenuFramework>(GameInstance));
// 	// if (MainMenuGameInstance)
// 	// {
// 	// 	return GameInstance;
// 	// }
// 	// else
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Function:[%s] GameInstance convert to II_GI_MenuFramework failed. Forgot changing the Game Instance?\
// 	// 		 We need Game Instance that dirived from [II_GI_MenuFramework] interface."), ANSI_TO_TCHAR(__FUNCTION__));
// 	// 	return nullptr;
// 	// }
// 	return (UObject*)GetFrameworkGameInstance_CPP();
// 			
// }

II_GI_MenuFramework* UUIHandlerBase::GetFrameworkGameInstance_CPP()
{
	if (FrameworkGameInstance_CPP)
	{
		return FrameworkGameInstance_CPP;
	}
	
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
