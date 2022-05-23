// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUI_FrameWork/UI_Modules/UI_Save/SaveGameObjs/SingleObjectSaveGameObjBase.h"
#include "UObject/Interface.h"
#include "I_CustomSave.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UI_CustomSave : public UInterface
{
	GENERATED_BODY()
};

/**
 * 由想被本存储系统保存的Actor所实装
 */
class BASEUI_MODULE_API II_CustomSave
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief （不要覆盖，有默认实现）暴露给USaveBaseHandler使用，代表某个Actor从序列化过后的【不同Actor的保存信息】 恢复成对象的函数
	 * @param InByteArray 
	 */
	void LoadFromByteArray_CPP(TArray<uint8> InByteArray);

	
	/**
	 * @brief （不要覆盖，有默认实现）暴露给USaveBaseHandler使用，拿到序列化过后的【不同Actor的保存信息】，并存储到游戏存档中。
	 * @return 用于保存的 序列化过后的【不同Actor的保存信息】
	 */
	void GetSaveByteArray_CPP(TArray<uint8>& OutSaveArray);

protected:

	/**
	 * @brief 在Actor类覆盖并实现具体的【把Actor的状态保存到】。
	 *			所需要的信息已经在【LoadFromByteArray_CPP】函数中赋给了【不同Actor的保存信息（USingleObjectSaveGameObjBase的子类）】。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|Custom Save Interface", meta=(ToolTip="子类覆盖并实现具体的【从保存的信息中恢复Actor的状态】。所需要的信息已经在【LoadFromByteArray_CPP】函数中赋给了【不同Actor的保存信息（USingleObjectSaveGameObjBase的子类）】。"))
	void DoActorSave();

	/**
	 * @brief 在Actor类覆盖并实现具体的【从保存的信息中恢复Actor的状态】。
	 *			所需要的信息已经在【LoadFromByteArray_CPP】函数中赋给了【不同Actor的保存信息（USingleObjectSaveGameObjBase的子类）】。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|Custom Save Interface", meta=(ToolTip="子类覆盖并实现具体的【从保存的信息中恢复Actor的状态】。所需要的信息已经在【LoadFromByteArray_CPP】函数中赋给了【不同Actor的保存信息（USingleObjectSaveGameObjBase的子类）】。"))
	void DoActorRecoverSavedData();
};
