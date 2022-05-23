// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"

#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GUI_FrameWork/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"

void UResumeMenuBaseHandler::ResumeGame_Implementation(UWidget* InControlledWidget)
{
	ResumeGame_CPP(InControlledWidget);
}

void UResumeMenuBaseHandler::ExitToMainMenu_Implementation()
{
	ExitToMainMenu_CPP();
}

void UResumeMenuBaseHandler::ExitToDesktop_Implementation()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UResumeMenuBaseHandler::ResumeGame_CPP(UWidget* InControlledWidget)
{
	if (IsValid(InControlledWidget))
	{
		InControlledWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] ControlledResumeMenu variable is NOT set."), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void UResumeMenuBaseHandler::ExitToMainMenu_CPP()
{
	II_GI_MenuFramework* MainMenuGameInstance = (II_GI_MenuFramework*)GetFrameworkGameInstance().GetInterface();
	if (MainMenuGameInstance)
	{
		auto MainMenuMap = MainMenuGameInstance->GetMainMenuMap();
		FString MapName = MainMenuMap->GetMapName();
		UGameplayStatics::OpenLevel(this, FName(MainMenuMap->GetMapName()));
	}
}

void UResumeMenuBaseHandler::ExitToDesktop_CPP()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UResumeMenuBaseHandler::SetControlledMenu(UWidget* InControlledResumeMenu)
{
	ControlledResumeMenu = InControlledResumeMenu;
}
