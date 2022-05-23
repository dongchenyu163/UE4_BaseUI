#pragma once

#include "CoreMinimal.h"
#include "LowLevelHandler/SaveLoad/Interfaces/I_Save.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


struct FSaveGameActorArchiveProxy : public FArchiveProxy
{
	FSaveGameActorArchiveProxy(FArchive& InInnerArchive)
		: FArchiveProxy(InInnerArchive)
	{
		ArIsSaveGame = true;
	}

	virtual FArchive& operator<< (AActor* Value)
	{
		UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("========================================"));
        UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("Function:[%s] Serialize the Actor [%s] !"), ANSI_TO_TCHAR(__FUNCTION__), *Value->GetName());
		
		// 【反射系统变量】的保存和恢复
        // 通过反射系统来恢复或保存 加入到反射系统的变量
        for (TFieldIterator<FProperty> PropertyIt(Value->GetClass(), EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
        {
            UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("Function:[%s] ----> Iterate the property [%s] !"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyIt->GetName());

        	/// ShouldSerializeValue() True的条件：
        	/// 1. Archive不忽略 本Property
        	/// 2. Property含有SaveGame标志位，Archive是【检查SaveGame标志位】的Archive.
        	/// 3. Property不包含CPF_Transient CPF_DuplicateTransient CPF_NonPIEDuplicateTransient CPF_NonTransactional CPF_Deprecated CPF_DevelopmentAssets CPF_SkipSerialization
        	/// 4. 太复杂。。。/Engine/Source/Runtime/CoreUObject/Private/UObject/Property.cpp#L871-L871
            if (!PropertyIt->ShouldSerializeValue(*this))
            {
            	UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("Function:[%s] Property [%s] has NO [SaveGame] flag, DO NOT save to file!"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyIt->GetName());
            	continue;
            }
            // 处理UObject，
            if(FObjectProperty* ObjectProperty = CastField<FObjectProperty>(*PropertyIt))
            {
            	if (ObjectProperty->ShouldSerializeValue(*this))
            	{
            		auto PtrPtr = ObjectProperty->ContainerPtrToValuePtr<UObject*>(this);
            		if (InnerArchive.IsLoading())
            		{
            			auto Obj = NewObject<UObject>(GetTransientPackage());
            			ObjectProperty->SetPropertyValue( PtrPtr, Obj );
            		}
            		(*PtrPtr)->Serialize(InnerArchive);
            	}
                else
                {
            		UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("Function:[%s] Property [%s] is UObject type, NOT save it! UObject类型物体无法正常保存，除非自己重写Serialize函数"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyIt->GetName());
            		continue;
                }
            }
            FStructuredArchiveFromArchive StructAr(InnerArchive);
            PropertyIt->SerializeItem(StructAr.GetSlot(), PropertyIt->ContainerPtrToValuePtr<void>(Value));
            UE_LOG(LogDongSavingSystem, VeryVerbose, TEXT("Function:[%s] ==== Property [%s] serialized to uint8 array!"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyIt->GetName());
		}

		return InnerArchive;
	}

	// 下面8个覆盖函数和父类一样，只是为了解决编辑的警告。
	virtual FArchive& operator<<(FName& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FText& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(UObject*& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FLazyObjectPtr& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FSoftObjectPath& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override { InnerArchive << Value; return *this; }
	virtual FArchive& operator<<(FField*& Value) override { InnerArchive << Value; return *this; }
};
