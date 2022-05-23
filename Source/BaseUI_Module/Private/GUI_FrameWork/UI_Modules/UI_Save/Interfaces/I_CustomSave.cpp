// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/UI_Modules/UI_Save/Interfaces/I_CustomSave.h"

#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/MultiUserSaveSystem/DongSaveSystemStatic.h"


// Add default functionality here for any II_CustomSave functions that are not pure virtual.
void II_CustomSave::LoadFromByteArray_CPP(TArray<uint8> InByteArray)
{
	// 反序列化InByteArray，拿到【保存本Actor信息】 类的对象。
	AActor* OperatingActor = dynamic_cast<AActor*>(this);
	UDongSaveSystemStatic_Simple::LoadActorFromMemory(InByteArray, OperatingActor);
	Execute_DoActorRecoverSavedData(OperatingActor);
}


void II_CustomSave::GetSaveByteArray_CPP(TArray<uint8>& OutSaveArray)
{
	//// WARINING: 卧槽！！ (AActor*)this 会导致转换过后的地址产生偏移。原始0x0010，转换过后是0x0020。。。原因未知。
	//// AActor* Obj2 = (AActor*)this;
	AActor* OperatingActor = dynamic_cast<AActor*>(this);
	
	// 调用（通知）蓝图进行保存。
	Execute_DoActorSave(OperatingActor);

	// 序列化 【保存本Actor信息】 类的对象并返回。
	UDongSaveSystemStatic_Simple::SaveActorToMemory(OperatingActor, OutSaveArray);
}
