// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassesAndStructs/FFunctionHandlerClassCustomizer.h"
#include "ClassesAndStructs/FunctionHandlerInfo.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "UObject/Object.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "MainMenuGameInstanceConfig.generated.h"

/**
 * 
 */
UCLASS(Config="MainMenuGameInstanceConfig")
class BASEUI_MODULE_API UMainMenuGameInstanceConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	// 指定本Config显示在【编辑器设置】还是【工程设置】中；分别对应两种选择【Editor】和【Project】
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	// Setting的左侧栏的大分类
	virtual FName GetCategoryName() const override { return TEXT("Dong UI Framework Modules Config"); }
	// 内部唯一标识符
	virtual FName GetSectionName() const override { return TEXT("Dong UI Framework Modules GameInstance Config"); }
	// Setting的左侧栏的子分类
	virtual FText GetSectionText() const override { return NSLOCTEXT("MainMenuGameInstanceConfig", "SectionText", "Game Instance Config"); }
	// Setting的左侧栏的悬停提示文字
	virtual FText GetSectionDescription() const override { return NSLOCTEXT("MainMenuGameInstanceConfig", "SectionDescriptionText", "配置GameInstance"); }

	UMainMenuGameInstanceConfig()
		: Super()
	{
		// UI Handlers
		HandlerClassDict.Add(FName("MapSelector"), FFunctionHandlerInfo(UMapSelectionBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("NextLevelHandler"), FFunctionHandlerInfo(UNextLevelBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("ResumeMenuHandle"), FFunctionHandlerInfo(UResumeMenuBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("SaveHandler"),
			// FFunctionHandlerInfo(USaveBaseHandler::StaticClass(), (TMap<FString, UClass*>*)&USaveBaseHandler::Map_Purpose_To_DependenceHandlerClass,
			FFunctionHandlerInfo(USaveBaseHandler::StaticClass(),
			TMap<FString, FName>({
				TPair<FString, FName>("LowLevelSaveHandler", "LowLevelSaveHandler"),
				TPair<FString, FName>("UserManager", "UserManager"),
			})
			));
		HandlerClassDict.Add(FName("SinglePlayerMenuHandle"),
			// FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(), (TMap<FString, UClass*>*)&USinglePlayerMenuBaseHandler::Map_Purpose_To_DependenceHandlerClass,
			FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(),
			TMap<FString, FName>({
				TPair<FString, FName>("MapSelector", "MapSelector"),
				TPair<FString, FName>("SaveHandler", "SaveHandler"),
			})
			));
		// Low level Handlers
		HandlerClassDict.Add(FName("LowLevelSaveHandler"),
			FFunctionHandlerInfo(USavingBaseHandler::StaticClass(),
				TMap<FString, FName>(
					{
					TPair<FString, FName>("UserManager", "UserManager"),
					}
				)
			)
		);
		HandlerClassDict.Add(FName("UserManager"), FFunctionHandlerInfo(UUserManagerBase::StaticClass()));
		// UNextLevelBaseHandler::Map_Purpose_To_DependenceHandlerClass;
		ReloadConfig();
	}
	
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override
	{
		HandlerClassDict.GetKeys(HandlerNameList);
		UReflectionOperations::SetNewOptionsToEnum("EFunctionHandlerNames_BP", HandlerNameList);
	}
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		HandlerClassDict.GetKeys(HandlerNameList);
		UReflectionOperations::SetNewOptionsToEnum("EFunctionHandlerNames_BP", HandlerNameList);

		if (FunctionHandlerClassCustomizer.IsValid())
		{
			FunctionHandlerClassCustomizer->Handler_OnConfigChanged();
		}
	}

	virtual void PostCDOContruct()
	{
		FPropertyEditorModule& PropertyEditorModuleRef = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModuleRef.RegisterCustomClassLayout("MainMenuGameInstanceConfig", FOnGetDetailCustomizationInstance::CreateUObject(this, &UMainMenuGameInstanceConfig::MakeFunctionHandlerClassCustomizerInstance));
	}

	
	static UMainMenuGameInstanceConfig* Get() { return GetMutableDefault<UMainMenuGameInstanceConfig>(); }

	TSharedPtr<FFunctionHandlerClassCustomizer> FunctionHandlerClassCustomizer;
	TSharedRef<class IDetailCustomization> MakeFunctionHandlerClassCustomizerInstance()
	{
		auto ReturnDetailCustomizationRef = FFunctionHandlerClassCustomizer::MakeInstance();
		FunctionHandlerClassCustomizer = StaticCastSharedRef<FFunctionHandlerClassCustomizer>(ReturnDetailCustomizationRef);
		return ReturnDetailCustomizationRef;
	}
	
	/**
	 * @brief 测试 【依赖的Handler的名称 所对应的 Handler类别】 和 【各个Handler类中定义的依赖Handler的类型】是否相同。
	 * @return 是否正确
	 */
	bool CheckHandlerDependencyValid()
	{
		bool bDependencyIsValid = true;

		// 循环所有被配置的Handler
		for(auto FunctionHandlerInfoPair
			: HandlerClassDict)
		{
			const size_t DependencyCount = FunctionHandlerInfoPair.Value.DependencyHandlerNameCollection.Num();
			if (DependencyCount == 0)
			{
				continue;
			}
			// 循环当前被配置的Handler的依赖列表
			for (FName DependentName : FunctionHandlerInfoPair.Value.DependencyHandlerNameCollection)
			{
				// 从依赖Handler的名称，查询依赖的目的
				const FString* DependentPurposePtr = FunctionHandlerInfoPair.Value.Map_Purpose_To_HandlerName.FindKey(DependentName);
				if (UNLIKELY(!DependentPurposePtr))
				{
					UE_LOG(LogTemp, Display, TEXT("Function:[%s] HandlerPurpose can not be found by dependent handler name: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *DependentName.ToString());
					return false;
				}

				// 原始Handler的 依赖的目的 查询 （原始Handler中定义的）该依赖所需的Class.
				const UClass* HandlerNeededClass = (*FunctionHandlerInfoPair.Value.Map_Purpose_To_DependenceHandlerClassPtr)[*DependentPurposePtr];
				// 在被配置的Handler中 查找 对应 依赖的Class.
				const FFunctionHandlerInfo* SettedHandlerInfoPtrPtr = HandlerClassDict.Find(DependentName);
				if (UNLIKELY(SettedHandlerInfoPtrPtr == nullptr))
				{
					UE_LOG(LogTemp, Display, TEXT("Function:[%s] Dependent handler name: [%s] is not set in Project Settings"), ANSI_TO_TCHAR(__FUNCTION__), *DependentName.ToString());
					return false;
				}
				const UClass* SettedHandlerClass = SettedHandlerInfoPtrPtr->HandlerClass;

				if (SettedHandlerClass != HandlerNeededClass)
				{
					UE_LOG(LogTemp, Error, TEXT("Function:[%s] Handler: [%s] is dependent [%s] type handler with purpose [%s]; But you give a handler with name [%s], which is [%s] type!!!"),
						ANSI_TO_TCHAR(__FUNCTION__),
						*FunctionHandlerInfoPair.Key.ToString(),
						**DependentPurposePtr,
						*HandlerNeededClass->GetClass()->GetName(),
						*DependentName.ToString(),
						*HandlerClassDict[DependentName].HandlerClass->GetClass()->GetName()
						);
					return false;
				}
			}
			
		}
		return bDependencyIsValid;
	}
public:
	TArray<FName> HandlerNameList;
public:
	UPROPERTY(EditAnywhere, Config, BlueprintReadOnly, Category="UI Handler Settings", meta=(DisplayName="Handler use in game", ToolTip="程序中使用FName来查找并引用对应模块"))
	TMap<FName, FFunctionHandlerInfo> HandlerClassDict;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info")
	UDataTable* DT_MapInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info", meta=(Tooltip="作为主菜单的World资产"))
	TSoftObjectPtr<UWorld> MainMenuMap;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Info")
	// TSubclassOf<UUser>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="User Global Data", meta=(DisplayName="User Global Data Class", Tooltip="用户存档"))
	TSubclassOf<UUserGlobalDataSaveGameObjBase> UserGlobalData_SaveGameObjClass = UUserGlobalDataSaveGameObjBase::StaticClass();
};
