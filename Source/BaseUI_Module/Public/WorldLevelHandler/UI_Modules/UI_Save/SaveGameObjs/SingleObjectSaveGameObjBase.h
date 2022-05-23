// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SingleObjectSaveGameObjBase.generated.h"

/**
 * 每一个想要保存的UObject（主要是AActor类型）都需要继承I_CustomSave接口，
 *		并添加【本类 或者 本类的子类】类型的成员变量（UPROPERTY(SaveGame)宏标记）。
 *			原因：序列化我使用了异步操作，防止本UObject被GC回收。
 * 本类的使用方式：
 *		针对每一种保存的情况：
 *			比如：要保存的一场游戏中，
 *			【ActorA类】只需要保存【位置】以及【自定义的一些结构体A数据】
 *			【ActorB类】需要保存【姿态】以及【自定义的一些结构体B数据】
 *			以上两种就是两种不同的保存情况。
 *			
 *		需要继承本类并添加想保存的成员变量，同样需要添加UPROPERTY(SaveGame)宏标记
 *
 *	原理以及位置
 */
UCLASS(Blueprintable, BlueprintType, meta=(ToolTip=""))
class BASEUI_MODULE_API USingleObjectSaveGameObjBase : public USaveGame
{
	GENERATED_BODY()
};
