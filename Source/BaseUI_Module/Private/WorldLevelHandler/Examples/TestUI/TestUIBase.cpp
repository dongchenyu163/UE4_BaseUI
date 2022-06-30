// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/Examples/TestUI/TestUIBase.h"
#include "WorldLevelHandler/NormalHandlers/MapsInfoHandler/MapsInfoHandler.h"
#include "BaseClassesAndTypes/BaseUI_Static.h"
#include "Kismet/GameplayStatics.h"

void UTestUIBase::OnBeginPlay_Implementation()
{
}

void UTestUIBase::Handle_OnComponentSwitchToNewWidget_Implementation()
{
	
	if (InGameMenu_BackgroundBlur)
	{
		UFunctionHandlerBase* FoundHandler;
		if (UBaseUI_Static::FindHandlerByName("MapsInfoHandler", FoundHandler))
		{
			UMapsInfoHandler* MapSelectionHandlerPtr = dynamic_cast<UMapsInfoHandler*>(FoundHandler);
			const FMapInfo* MapInfoPtr = MapSelectionHandlerPtr->GetPlayingMapInfo_CPP();
			if (MapInfoPtr->bIsGameWorld)
			{
				InGameMenu_BackgroundBlur->SetBlurStrength(5);
			}
			else
			{
				InGameMenu_BackgroundBlur->SetBlurStrength(0);
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [MapsInfoHandler]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}
}

TScriptInterface<II_GI_MenuFramework> UTestUIBase::GetFrameworkGameInstance()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(this);
	II_GI_MenuFramework* MainMenuGameInstance = Cast<II_GI_MenuFramework>(GameInstance);
	if (MainMenuGameInstance)
	{
		return GameInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] GameInstance convert to II_GI_MenuFramework failed. Forgot changing the Game Instance?\
			 We need Game Instance that dirived from [II_GI_MenuFramework] interface."), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}
			
}


II_GI_MenuFramework* UTestUIBase::GetFrameworkGameInstance_CPP()
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

