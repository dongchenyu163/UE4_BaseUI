// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/DA_WidgetInfo.h"
#include "Components/ActorComponent.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "AC_CTRL_MenuBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASEUI_MODULE_API UAC_CTRL_MenuBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAC_CTRL_MenuBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	UFUNCTION(BlueprintCallable, Category="DongMenuFramework")
	TScriptInterface<II_GI_MenuFramework> GetFrameworkGameInstance();
	II_GI_MenuFramework* GetFrameworkGameInstance_CPP();
	
	UFUNCTION(BlueprintCallable, Category="Widget")
	void Init(UDA_WidgetInfo* InInitWidgetInfo);
	
	UFUNCTION(BlueprintCallable, Category="Widget")
	void SwitchWidgetTo(UDA_WidgetInfo* InNewWidget, bool bInChangeWorld = false);

	UFUNCTION(BlueprintCallable, Category="Widget")
	UUserWidget* CreateNewWidget(UDA_WidgetInfo* InWidgetInfo);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget", meta=(ToolTip=""))
	UDA_WidgetInfo* InitialWidgetInfo;
	
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(ToolTip="正在显示的Widget"))
	UDA_WidgetInfo* ShowingWidgetInfo;

};
