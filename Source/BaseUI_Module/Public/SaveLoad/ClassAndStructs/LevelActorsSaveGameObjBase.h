// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveLoad/ClassAndStructs/SaveGameObjBase.h"
#include "GUI_FrameWork/UI_Modules/UI_Save/Config/UISaveLoadSystemConfig.h"
#include "GUI_FrameWork/UI_Modules/UI_Save/Interfaces/I_UI_Savable.h"
#include "SaveLoad/Archives/MemoryWriter_UObject.h"
#include "UObject/Object.h"
#include "LevelActorsSaveGameObjBase.generated.h"


/**
 * FActorSavingInfo用在TArray中
 * 使用反射系统序列化的话是没法调用【operator<<】方法
 */
USTRUCT(BlueprintType)
struct FActorSavingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category="Save Load System|Saving Data|Actor Data")
	TArray<uint8> ObjectByteArray;
	
	UPROPERTY(EditAnywhere, SkipSerialization, BlueprintReadWrite, Category="Save Load System|Saving Data|Actor Data")
	UClass* ActorClass;
	
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category="Save Load System|Saving Data|Actor Data", meta=(ToolTip="记录当前Actor的名称（ID）用于在Loading的时候判断现在关卡中有没有同名Actor；关卡初始就有的Actor，就不需要Spawn一个新的了"))
	FName ActorName;

	bool Serialize (FArchive& Ar)
	{
		Ar << *this;
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] FActorSavingInfo Serialize"), ANSI_TO_TCHAR(__FUNCTION__));
		return true;
	}
	
	friend FArchive& operator<< (FArchive& Ar, FActorSavingInfo& InStruct)
	{
		Ar << InStruct.ObjectByteArray;
		
		if (Ar.IsLoading())
		{
			FString ClassName = "";
			Ar << ClassName;
			UClass* ActorClass = FindObject<UClass>(ANY_PACKAGE, *ClassName);
			InStruct.ActorClass = ActorClass;
		}
		else
		{
			FString ClassName = InStruct.ActorClass->GetName();
			Ar << ClassName;
		}

		UE_LOG(LogTemp, Display, TEXT("Function:[%s] FActorSavingInfo Serialize"), ANSI_TO_TCHAR(__FUNCTION__));

		Ar << InStruct.ActorName;
		return Ar;
	}
};

/**
 * @brief 用于提示【反射系统】在序列化的时候使用FActorSavingInfo的
 *			bool Serialize (FArchive& Ar)
 *			形式的函数
 */
template<>
struct TStructOpsTypeTraits<FActorSavingInfo> : public TStructOpsTypeTraitsBase2<FActorSavingInfo>
{
	enum
	{
		WithSerializer = true
	};
};


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(ToolTip="由于保存某关卡内部【全局信息】和【所有需要保存的Actor】的信息的Object"))
class BASEUI_MODULE_API ULevelActorsSaveGameObjBase : public USaveGameObjBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, SkipSerialization, BlueprintReadWrite, Category="Save Load System|Saving Data", meta=(Tooltip="本存档的显示信息"))
	USaveGameObjectHeaderBase* PlayingInfo;
	
	UPROPERTY(SaveGame, meta=(ToolTip="所有要保存的物体的数据；反射系统不支持嵌套数组，只能用1维数组来存储所有的"))
	TArray<FActorSavingInfo> LevelObjectInfoList;
	
	// virtual void Serialize(FArchive& Ar) override
	// {
	// 	if (Ar.IsLoading())
	// 	{
	// 		PlayingInfo = NewObject<USaveGameObjectHeaderBase>(USaveLoadSystemConfig::Get()->SaveHeaderClass);
	// 	}
	// 	PlayingInfo->Serialize(Ar);
	// 	Ar << LevelObjectInfoList;
	// }
};
