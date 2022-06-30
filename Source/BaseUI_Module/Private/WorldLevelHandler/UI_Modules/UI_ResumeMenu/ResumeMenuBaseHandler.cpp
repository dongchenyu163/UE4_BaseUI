// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"

#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"

const FFunctionHandlerDef UResumeMenuBaseHandler::HandlerDef(StaticClass(), {
   HandlerDependentPair("MapsInfoHandler", new FFunctionHandlerDependent(UMapsInfoHandler::StaticClass(),
	   NSLOCTEXT("UNextLevelBaseHandler", "MapsInfoHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
});

void UResumeMenuBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	MapsInfoHandler = dynamic_cast<UMapsInfoHandler*>(Map_Purpose_To_HandlerInstance["MapsInfoHandler"]);
}

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
	const auto MainMenuMap = MapsInfoHandler->GetMainMenuMap();
	FString MapName = MainMenuMap->GetMapName();
	UGameplayStatics::OpenLevel(this, FName(MainMenuMap->GetMapName()));
}

void UResumeMenuBaseHandler::ExitToDesktop_CPP()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UResumeMenuBaseHandler::SetControlledMenu(UWidget* InControlledResumeMenu)
{
	ControlledResumeMenu = InControlledResumeMenu;
}
