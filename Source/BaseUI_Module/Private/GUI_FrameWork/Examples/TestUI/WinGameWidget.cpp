// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/Examples/TestUI/WinGameWidget.h"

void UWinGameWidget::LoadNextLevel_CPP()
{
	auto GameInstance = GetFrameworkGameInstance_CPP();
	GameInstance->GetNextLevelHandler()->LoadNextLevel_CPP();
}

void UWinGameWidget::ExitToMainMenu_CPP()
{
	auto GameInstance = GetFrameworkGameInstance_CPP();
	GameInstance->GetResumeMenuBaseHandler_CPP()->ExitToMainMenu_CPP();
}

void UWinGameWidget::ExitToDesktop_CPP()
{
	auto GameInstance = GetFrameworkGameInstance_CPP();
	GameInstance->GetResumeMenuBaseHandler_CPP()->ExitToDesktop_CPP();
}
