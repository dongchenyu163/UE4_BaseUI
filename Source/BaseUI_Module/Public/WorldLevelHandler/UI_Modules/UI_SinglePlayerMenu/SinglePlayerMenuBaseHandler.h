// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/Interfaces/I_UI_Savable.h"
#include "Interfaces/I_UI_SinglePlayerFunctions.h"
#include "UObject/Object.h"
#include "SinglePlayerMenuBaseHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API USinglePlayerMenuBaseHandler : public UUIHandlerBase, public II_UI_SinglePlayerFunctions
{
	GENERATED_BODY()
public:
	USinglePlayerMenuBaseHandler() {}
	USinglePlayerMenuBaseHandler(UObject* InMapSelectableObj) { this->MapSelectableObj = InMapSelectableObj; }
protected:
	virtual ~USinglePlayerMenuBaseHandler() override {}

protected:
	virtual void AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr) override;
	virtual void AssignDependentHandlerPtr() override;
	
public:
	
	virtual void StartNewGame_Implementation() override;
	virtual void ContinueFromLast_Implementation() override;

	virtual void StartNewGame_CPP() override;
	virtual void ContinueFromLast_CPP() override;
	// UFUNCTION(BlueprintNativeEvent)
	void SetMapSelectableObj_CPP(UObject* InMapSelectableObj) { this->MapSelectableObj = InMapSelectableObj; }

	void SetSaveBaseHandler(UObject* InSaveBaseHandler) { SaveBaseHandler = InSaveBaseHandler; }

	void SetMapInfoDataTable(UDataTable* InMapInfoDataTable) { MapInfoDataTable = InMapInfoDataTable; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapSelection", meta=(Tooltip=""))
	UDataTable* MapInfoDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapSelection", meta=(Tooltip=""))
	TScriptInterface<II_UI_MapSelectable> MapSelectableObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapSelection", meta=(Tooltip=""))
	TScriptInterface<II_UI_Savable> SaveBaseHandler;

public:
	const static FFunctionHandlerDef HandlerDef;

};
