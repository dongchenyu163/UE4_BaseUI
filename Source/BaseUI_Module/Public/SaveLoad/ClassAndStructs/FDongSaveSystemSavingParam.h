#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveLoad/SaveGameObjectHeaderBase.h"
#include "FDongSaveSystemSavingParam.generated.h"


USTRUCT(BlueprintType)
struct BASEUI_MODULE_API FDongSaveSystemSavingParam
{
	GENERATED_BODY()

	UPROPERTY(SkipSerialization, meta=(ToolTip="主要存储的UObject"))
	USaveGame* MainSaveObj;
	UPROPERTY(SkipSerialization, meta=(ToolTip="文件头，显示存档列表的时候会读取"))
	USaveGameObjectHeaderBase* SaveObjHeader;

	UPROPERTY(meta=(ToolTip="要存储的区域的名称。需要和ProjectSetting中设置的名称相同"))
	FName SaveAreaID;
	UPROPERTY(meta=(ToolTip=""))
	int32 SlotNum = INDEX_NONE;
	UPROPERTY(meta=(ToolTip=""))
	FString CustomSlotName;
	UPROPERTY(meta=(ToolTip=""))
	FString SaveAdditionalPath;

	TEnumAsByte<ESaveGameType_Candidate> SaveType = NumberSave;

	void AsNumberSave(FName InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader, int32 InSlotNum) 
	{
		MainSaveObj = InMainSaveObj;
		SaveObjHeader = InSaveObjHeader;
		SaveType = NumberSave;
		
		SaveAreaID = InSaveAreaID;
		SlotNum = InSlotNum;
	}
	
	void AsRollingSave(FName InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader) 
	{
		MainSaveObj = InMainSaveObj;
		SaveObjHeader = InSaveObjHeader;
		SaveType = RollingSave;
		
		SaveAreaID = InSaveAreaID;
	}
	
	void AsCustomNameSave(FName InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader, FString InCustomSlotName, FString InSaveAdditionalPath="") 
	{
		MainSaveObj = InMainSaveObj;
		SaveObjHeader = InSaveObjHeader;
		SaveType = CustomNameSave;
		
		SaveAreaID = InSaveAreaID;
		CustomSlotName = InCustomSlotName;
		SaveAdditionalPath = InSaveAdditionalPath;
	}
	
	void AsOfficialSave(FName InSaveAreaID, USaveGame* InMainSaveObj, FString InCustomSlotName) 
	{
		MainSaveObj = InMainSaveObj;
		SaveType = OfficialSave;
		
		SaveAreaID = InSaveAreaID;
		CustomSlotName = InCustomSlotName;
	}

	/**
	 * @brief 生成用于判断 某个SaveArea下的 某个存档名称 是否存在。
	 * @param InSaveAreaID 
	 * @param InSlotNum 
	 * @param InCustomSlotName 
	 */
	void AsIsSlotNameExist(FName InSaveAreaID, int32 InSlotNum, FString InCustomSlotName)
	{
		SaveAreaID = InSaveAreaID;
		SlotNum = InSlotNum;
		CustomSlotName = InCustomSlotName;
	}
	
};

UCLASS()
class USaveSystemSavingParamStatic : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="SavingParam")
	static void MakeNumberSaveParam(ESaveArea_BP InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader, int32 InSlotNum, FDongSaveSystemSavingParam& OutParam)
	{
		const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID));
		OutParam.AsNumberSave(SaveAreaID, InMainSaveObj, InSaveObjHeader, InSlotNum);
	}

	UFUNCTION(BlueprintCallable, Category="SavingParam")
	static void MakeRollingSaveParam(ESaveArea_BP InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader, FDongSaveSystemSavingParam& OutParam) 
	{
		const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID));
		OutParam.AsRollingSave(SaveAreaID, InMainSaveObj, InSaveObjHeader);
	}
	
	UFUNCTION(BlueprintCallable, Category="SavingParam")
	static void MakeCustomNameSaveParam(ESaveArea_BP InSaveAreaID, USaveGame* InMainSaveObj, USaveGameObjectHeaderBase* InSaveObjHeader, FString InCustomSlotName, FString InSaveAdditionalPath, FDongSaveSystemSavingParam& OutParam) 
	{
		const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID));
		OutParam.AsCustomNameSave(SaveAreaID, InMainSaveObj, InSaveObjHeader, InCustomSlotName, InSaveAdditionalPath);
	}
	
	UFUNCTION(BlueprintCallable, Category="SavingParam")
	static void MakeOfficialSaveParam(ESaveArea_BP InSaveAreaID, USaveGame* InMainSaveObj, FString InCustomSlotName, FDongSaveSystemSavingParam& OutParam) 
	{
		const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID));
		OutParam.AsOfficialSave(SaveAreaID, InMainSaveObj, InCustomSlotName);
	}
};

