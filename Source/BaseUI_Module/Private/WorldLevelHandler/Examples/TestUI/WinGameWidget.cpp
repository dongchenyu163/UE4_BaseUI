// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/Examples/TestUI/WinGameWidget.h"

#include "BaseClassesAndTypes/BaseUI_Static.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"

void UWinGameWidget::LoadNextLevel_CPP()
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("NextLevelHandler", FoundHandler))
	{
		UNextLevelBaseHandler* ResumeMenuBaseHandlerPtr = dynamic_cast<UNextLevelBaseHandler*>(FoundHandler);
		ResumeMenuBaseHandlerPtr->LoadNextLevel_CPP();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [ResumeMenuHandle]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void UWinGameWidget::ExitToMainMenu_CPP()
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("ResumeMenuHandle", FoundHandler))
	{
		UResumeMenuBaseHandler* ResumeMenuBaseHandlerPtr = dynamic_cast<UResumeMenuBaseHandler*>(FoundHandler);
		ResumeMenuBaseHandlerPtr->ExitToMainMenu_CPP();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [ResumeMenuHandle]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void UWinGameWidget::ExitToDesktop_CPP()
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
