#pragma once

#include "CoreMinimal.h"
#include "LowLevelHandler/SaveLoad/Interfaces/I_Save.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/UserGlobalDataSaveGameObjBase.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/UI/USaveSystemLoadingScreenBase.h"
#include "UISaveLoadSystemConfig.generated.h"

UCLASS(config = SaveLoadSystemConfig)
class UUISaveLoadSystemConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UUISaveLoadSystemConfig()
		: Super()
	{
		// 调用USaveLoadSystemConfig的 获取CDO的函数，没有的话引擎会帮忙创建。
		// 构造USaveLoadSystemConfig的时候，ESaveArea_BP就会被更新。
		// 本类的Config的加载是在这个构造函数结束后才进行，所以不需要额外做其他事，即可正常加载。
		//		Note：没有这句话的话，ESaveArea_BP违背更新，并处于空枚举的状态，无法从文件中正常恢复记录的值。
		USaveLoadSystemConfig::Get();
	}
	
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetCategoryName() const override { return TEXT("Dong UI Framework Modules Config"); }  // Setting的左侧栏的大分类
	virtual FName GetSectionName() const override { return TEXT("UI Save & Load System Config"); }  // Setting的左侧栏的子分类

	// Setting的左侧栏的子分类
	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT("UISaveLoadSystemConfig", "SectionText", "Dong UI Save Load System Config");
	}

	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT("UISaveLoadSystemConfig", "SectionDescriptionText", "设置上层存档系统（UI以及关卡相关）的参数");
	}

	// 单例模式，静态获取本对象。
	UFUNCTION(BlueprintPure, Category="Save Load System Config Object", DisplayName="GetSaveLoadSystemConfig")
	static UUISaveLoadSystemConfig* Get() { return GetMutableDefault<UUISaveLoadSystemConfig>(); }

	/**
	 * 设置项区域
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="User Global Data", meta=(DisplayName="User Global Data Class", Tooltip="用户存档"))
	TSubclassOf<UUserGlobalDataSaveGameObjBase> UserGlobalData_SaveGameObjClass = UUserGlobalDataSaveGameObjBase::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Misc", meta=(Tooltip="存档缩略图的像素大小"))
	FVector2D ThumbnailSize = FVector2D(384, 384);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Save Area ID", meta=(Tooltip="用户主动的存档所使用的存储区域"))
	TEnumAsByte<ESaveArea_BP> NormalSaveAreaID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Save Area ID", meta=(Tooltip="快速存档所使用的存储区域"))
	TEnumAsByte<ESaveArea_BP> QuickSaveAreaID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Save Area ID", meta=(Tooltip="程序自动存档所使用的存储区域"))
	TEnumAsByte<ESaveArea_BP> AutoSaveAreaID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="User Global Data", meta=(Tooltip="单个用户的信息存档所使用的存储区域"))
	TEnumAsByte<ESaveArea_BP> UserGlobalDataSaveAreaID;

	UPROPERTY(BlueprintReadWrite, Category="User Global Data", meta=(DisplayName="User Global Data Slot Name", Tooltip="存储某个用户的整体进度的存档Slot名称"))
	FString UserGlobalData_SlotNameString = "UserGlobalData";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Misc", meta=(Tooltip="通过【SetSaveSystemLoadingScreenWidgetClass】更改当前加载界面的类型，以达到切换不同的加载界面"))
	TSubclassOf<UUSaveSystemLoadingScreenBase> SaveSystemLoadingScreenWidgetClass;

};