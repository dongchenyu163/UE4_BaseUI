// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldLevelHandler/Examples/TestUI/MainMenuWidget.h"
#include "BaseClassesAndTypes/BaseUI_Static.h"

#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"

void UMainMenuWidget::StartNewGame_Implementation()
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("SinglePlayerMenuHandle", FoundHandler))
	{
		USinglePlayerMenuBaseHandler* SinglePlayerMenuBaseHandlerPtr = dynamic_cast<USinglePlayerMenuBaseHandler*>(FoundHandler);
		SinglePlayerMenuBaseHandlerPtr->StartNewGame_CPP();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [SinglePlayerMenuHandle]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void UMainMenuWidget::ContinueFromLast_Implementation()
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("SinglePlayerMenuHandle", FoundHandler))
	{
		USinglePlayerMenuBaseHandler* SinglePlayerMenuBaseHandlerPtr = dynamic_cast<USinglePlayerMenuBaseHandler*>(FoundHandler);
		SinglePlayerMenuBaseHandlerPtr->ContinueFromLast_CPP();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [SinglePlayerMenuHandle]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void UMainMenuWidget::ExitToDesktop_Implementation()
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("ResumeMenuHandle", FoundHandler))
	{
		UResumeMenuBaseHandler* ResumeMenuBaseHandlerPtr = dynamic_cast<UResumeMenuBaseHandler*>(FoundHandler);
		ResumeMenuBaseHandlerPtr->ExitToDesktop_CPP();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [ResumeMenuHandle]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
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
