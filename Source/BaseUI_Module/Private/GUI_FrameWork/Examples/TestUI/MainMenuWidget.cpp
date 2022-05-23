// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/Examples/TestUI/MainMenuWidget.h"

void UMainMenuWidget::StartNewGame_Implementation()
{
	// TScriptInterface<II_GI_MenuFramework> MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		// TScriptInterface<II_UI_SinglePlayerFunctions> SinglePlayerMenuBaseHandler = 
			// II_GI_MenuFramework::Execute_GetSinglePlayerMenuBaseHandler(MainMenuGameInstance.GetObject());
		// Execute_StartNewGame(SinglePlayerMenuBaseHandler.GetObject());
		MainMenuGameInstance->GetSinglePlayerMenuBaseHandler_CPP()->StartNewGame_CPP();
	}
}

void UMainMenuWidget::ContinueFromLast_Implementation()
{
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		// TScriptInterface<II_UI_SinglePlayerFunctions> SinglePlayerMenuBaseHandler = 
			// II_GI_MenuFramework::Execute_GetSinglePlayerMenuBaseHandler(MainMenuGameInstance.GetObject());
		// Execute_ContinueFromLast(SinglePlayerMenuBaseHandler.GetObject());
		MainMenuGameInstance->GetSinglePlayerMenuBaseHandler_CPP()->ContinueFromLast_CPP();
	}
}

void UMainMenuWidget::ExitToDesktop_Implementation()
{
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		// TScriptInterface<II_UI_SinglePlayerFunctions> SinglePlayerMenuBaseHandler = 
			// II_GI_MenuFramework::Execute_GetSinglePlayerMenuBaseHandler(MainMenuGameInstance.GetObject());
		// Execute_ExitToDesktop(SinglePlayerMenuBaseHandler.GetObject());
		MainMenuGameInstance->GetResumeMenuBaseHandler_CPP()->ExitToDesktop_CPP();
	}
}

void UMainMenuWidget::ResumeGame_CPP(UWidget* InControlledWidget)
{
}

void UMainMenuWidget::ExitToMainMenu_CPP()
{
}

void UMainMenuWidget::ExitToDesktop_CPP()
{
	ExitToDesktop_Implementation();
}

void UMainMenuWidget::StartNewGame_CPP()
{
	StartNewGame_Implementation();
}

void UMainMenuWidget::ContinueFromLast_CPP()
{
	ContinueFromLast_Implementation();
}
