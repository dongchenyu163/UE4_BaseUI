// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayClasses/ControllerComponent/AC_CTRL_MenuBase.h"
#include "WorldLevelHandler/UI_Interfaces/I_SetControllerComp.h"
#include "Utility_BP_Func.h"
#include "BaseClassesAndTypes/BaseUI_Static.h"
#include "Blueprint/UserWidget.h"
// #include "MainMenu/Examples/TestUI/TestUIBase.h"


// Sets default values for this component's properties
UAC_CTRL_MenuBase::UAC_CTRL_MenuBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_CTRL_MenuBase::BeginPlay()
{
	Super::BeginPlay();

	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("MapSelector", FoundHandler))
	{
		MapSelectionHandler = dynamic_cast<UMapSelectionBaseHandler*>(FoundHandler);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] No handler named [MapSelector]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	
// 	MapsInfoHandler
// UMapsInfoHandler
	if (UBaseUI_Static::FindHandlerByName("MapsInfoHandler", FoundHandler))
	{
		MapsInfoHandler = dynamic_cast<UMapsInfoHandler*>(FoundHandler);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] No handler named [MapsInfoHandler]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	
}


// Called every frame
void UAC_CTRL_MenuBase::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TScriptInterface<II_GI_MenuFramework> UAC_CTRL_MenuBase::GetFrameworkGameInstance()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(this);
	II_GI_MenuFramework* MainMenuGameInstance(Cast<II_GI_MenuFramework>(GameInstance));
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

II_GI_MenuFramework* UAC_CTRL_MenuBase::GetFrameworkGameInstance_CPP()
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

void UAC_CTRL_MenuBase::Init(UDA_WidgetInfo* InInitWidgetInfo)
{
	if (UUtility_BP_Func::ExecOnClient(this))
	{
		InInitWidgetInfo->WidgetRef = CreateNewWidget(InInitWidgetInfo);
		if (InInitWidgetInfo->WidgetRef && IsValid(InInitWidgetInfo->WidgetRef))
		{
			ShowingWidgetInfo = InInitWidgetInfo;
			InInitWidgetInfo->WidgetRef->AddToViewport();
		}
		else
		{
			return;
		}
	}
}

void UAC_CTRL_MenuBase::SwitchWidgetTo(UDA_WidgetInfo* InNewWidget, bool bInChangeWorld)
{
	if (UUtility_BP_Func::ExecOnClient(this))
	{
		if (IsValid(ShowingWidgetInfo))
		{
			ShowingWidgetInfo->WidgetRef->RemoveFromParent();
		}

		// 新切换的Widget是否需要切换地图（比如：返回主菜单被按下时候就需要切换世界）
		if (bInChangeWorld && !InNewWidget->AssociatedMapIdentifier.IsNone())
		{
			FMapInfo MapInfo;  // = *(GetFrameworkGameInstance_CPP()->GetMapUIInfo_CPP(InNewWidget->AssociatedMapIdentifier));
			MapsInfoHandler->GetMapInfo(InNewWidget->AssociatedMapIdentifier, MapInfo);
			
			FString CurrentMapString = GetWorld()->GetPackage()->FileName.ToString();
			if (MapInfo.IsValid())
			{
				FString DEBUG_LoadMap_AssetPathString = MapInfo.MapObject.GetLongPackageName();
				if (CurrentMapString != MapInfo.MapObject.GetLongPackageName())
				{
					MapSelectionHandler->LoadMap_CPP(&MapInfo);
					return;  // 切换地图会销毁当前的Controller，以及本Component。所以直接返回即可。
				}
			}
		}

		// 正常切换Widget
		// 
		const bool bValidWidgetPtr = InNewWidget->WidgetRef != nullptr;
		const bool bValidDataAssetObj = IsValid(InNewWidget->WidgetRef);
		if (!bValidWidgetPtr || !bValidDataAssetObj)
		{
			InNewWidget->WidgetRef = CreateNewWidget(InNewWidget);
			if (!InNewWidget->WidgetRef)
			{
				UE_LOG(LogTemp, Display, TEXT("Function:[%s] The widget with name: [%s] create failed."), ANSI_TO_TCHAR(__FUNCTION__), *InNewWidget->WidgetID.ToString());
				return;
			}
		}

		// 显示到屏幕，添加到Viewport。
		InNewWidget->WidgetRef->AddToViewport();

		// 通知目标UI
		II_SetControllerComp* WidgetInterface = Cast<II_SetControllerComp>(InNewWidget->WidgetRef);
		if (WidgetInterface)
		{
			WidgetInterface->GetOnComponentSwitchToNewWidgetDelegate_CPP()->Broadcast();
		}
		ShowingWidgetInfo = InNewWidget;
	}
}

UUserWidget* UAC_CTRL_MenuBase::CreateNewWidget(UDA_WidgetInfo* InWidgetInfo)
{
	if (UUtility_BP_Func::ExecOnClient(this))
	{
		auto PlayerController = Cast<APlayerController>(GetOwner());
		if (PlayerController)
		{
			InWidgetInfo->WidgetRef = CreateWidget(PlayerController, InWidgetInfo->WidgetClassRef, InWidgetInfo->WidgetID);
			II_SetControllerComp* MenuBaseWidget = Cast<II_SetControllerComp>(InWidgetInfo->WidgetRef);
			if (MenuBaseWidget)
			{
				MenuBaseWidget->SetUIObjectComponent_CPP(this);
			}
			else
			{
				auto ClassObj = InWidgetInfo->WidgetClassRef.GetDefaultObject();
				UE_LOG(LogTemp, Error, TEXT("Function:[%s] Widget Class Name: [%s] MUST Inherit [II_SetControllerComp] interface!"), ANSI_TO_TCHAR(__FUNCTION__), *(ClassObj->StaticClass()->GetName()));
				ensure(false);
				return nullptr;
			}
			return InWidgetInfo->WidgetRef;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] UAC_CTRL_MenuBase MUST insert to APlayerController class"), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] UI should NOT showing at the server."), ANSI_TO_TCHAR(__FUNCTION__));
	}
	return nullptr;
}

