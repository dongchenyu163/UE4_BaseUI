// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoad/MultiUserSaveSystem/DongSaveSystemStatic.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/Archives/FSaveGameActorArchiveProxy.h"
#include "SaveLoad/ClassAndStructs/LevelActorsSaveGameObjBase.h"
#include "SaveLoad/Interfaces/I_Save.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


bool UDongSaveSystemStatic_Simple::SaveGameToMemory(USaveGame* InSaveGameObject, USaveGameObjectHeaderBase* InSaveHeader, TArray<uint8>& OutSaveData)
{
	if (InSaveGameObject)
	{
		InSaveHeader->Init(InSaveGameObject->GetClass());

		FMemoryWriter MemoryWriter(OutSaveData, true);
		InSaveHeader->Serialize(MemoryWriter);
		
		// Then save the object state, replacing object refs and names with strings
		// FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
		InSaveGameObject->Serialize(MemoryWriter);

		return true; // Not sure if there's a failure case here.
	}

	return false;
}

USaveGame* UDongSaveSystemStatic_Simple::LoadGameFromMemory(const TArray<uint8>& InSaveData, FDongSaveSystemSavingParam& OutLoadingResult)
{
	// 检查传入参数是否合法。字节流长度应当大于0.
	if (InSaveData.Num() == 0)
	{
		// Empty buffer, return instead of causing a bad serialize that could crash
		return nullptr;
	}

	USaveGame* OutSaveGameObject = nullptr;
	FMemoryReader MemoryReader(InSaveData, true);

	// 读取文件头
	USaveGameObjectHeaderBase* SaveHeader = NewObject<USaveGameObjectHeaderBase>(GetTransientPackage());
	SaveHeader->Serialize(MemoryReader);
	OutLoadingResult.SaveObjHeader = SaveHeader;

	// 读取USaveGame的实际类型，创建新的对应的对象
	UClass* SaveGameClass = FindObject<UClass>(ANY_PACKAGE, *SaveHeader->SaveGameClassName);
	if (SaveGameClass == nullptr)
	{
		SaveGameClass = LoadObject<UClass>(nullptr, *SaveHeader->SaveGameClassName);
	}

	// 创建新的对应的对象合法，则从字节流进行加载。
	if (SaveGameClass != nullptr)
	{
		OutSaveGameObject = NewObject<USaveGame>(GetTransientPackage(), SaveGameClass);
		OutSaveGameObject->Serialize(MemoryReader);
	}

	if (OutSaveGameObject->GetClass()->IsChildOf(ULevelActorsSaveGameObjBase::StaticClass()))
	{
		const auto LevelActorsSaveGameObj = Cast<ULevelActorsSaveGameObjBase>(OutSaveGameObject);
		LevelActorsSaveGameObj->PlayingInfo = SaveHeader;
	}

	OutLoadingResult.MainSaveObj = OutSaveGameObject;
	return OutSaveGameObject;
}

bool UDongSaveSystemStatic_Simple::LoadHeaderObjFromMemoryBytes(const TArray<uint8>& InSaveData, USaveGameObjectHeaderBase*& OutHeaderData)
{
	if (InSaveData.Num() == 0)
	{
		// Empty buffer, return instead of causing a bad serialize that could crash
		return nullptr;
	}
	
	FMemoryReader MemoryReader(InSaveData, true);
	OutHeaderData->Serialize(MemoryReader);

	return true;
}

bool UDongSaveSystemStatic_Simple::ReadFileHeaderToMemoryBytes(const FString InFilePath, TArray<uint8>& OutSaveGameHeader)
{
	FScopedLoadingState ScopedLoadingState(*InFilePath);

	const uint32 Flags = 0;
	FArchive* Reader = IFileManager::Get().CreateFileReader( *InFilePath, Flags );
	if( !Reader )
	{
		UE_LOG(LogStreaming, Warning, TEXT("Failed to read file '%s' error."), *InFilePath);
		return false;
	}

	// 获取存储【自定义的存档文件头长度】的信息的长度。
	const int HeaderLengthDataSize = sizeof(uint64);
	// 【自定义的存档文件头长度】初始化Buffer数组。
	OutSaveGameHeader.Reset( HeaderLengthDataSize + 2 );
	OutSaveGameHeader.AddUninitialized( HeaderLengthDataSize );
	// 读取【自定义的存档文件头长度】
	Reader->Serialize(OutSaveGameHeader.GetData(), HeaderLengthDataSize);
	// 翻译读取的【自定义的存档文件头】信息到uint64类型。
	const int64 HeaderLength = *((int64*)OutSaveGameHeader.GetData());

	if (HeaderLength > Reader->TotalSize())
	{
		UE_LOG(LogDongSavingSystem, Warning, TEXT("Function:[%s] HeaderLength : [%lld] is larger than file size [%lld]!"), ANSI_TO_TCHAR(__FUNCTION__), HeaderLength, Reader->TotalSize());
	}

	FArchive* Reader2 = IFileManager::Get().CreateFileReader( *InFilePath, Flags );
	// 读取完整【文件头】部分。
	// const uint64 TotalSize = (int32)Reader->TotalSize();
	// Allocate slightly larger than file size to avoid re-allocation when caller null terminates file buffer
	OutSaveGameHeader.Reset( HeaderLength + 2 );
	OutSaveGameHeader.AddUninitialized( HeaderLength );
	Reader2->Serialize(OutSaveGameHeader.GetData(), HeaderLength);
	
	const bool Success = Reader->Close();
	delete Reader;	
	const bool Success2 = Reader2->Close();
	delete Reader2;
	return Success && Success2;
}

bool UDongSaveSystemStatic_Simple::UpdateHeader(const FString InFilePath, USaveGameObjectHeaderBase* OutHeaderData)
{
	// TODO：重命名UpdateHeader
	// TODO：完成这个Feature.
	return true;
}

bool UDongSaveSystemStatic_Simple::SaveGameToSlot(FDongSaveSystemSavingParam& InParam)
{
	TArray<uint8> ObjectBytes;
	bool bSerializedSuccess = false;

	// 检查 传入的InParam 是否合法
	if (!USaveLoadSystemConfig::CheckSaveSystemSavingParam(InParam))
	{
		return false;
	}

	// 获取保存的文件完整路径
	const FString&& SaveFullPath = GetPath(InParam);

	// 向Header写入存档文件名信息。
	if (IsValid(InParam.SaveObjHeader))
	{
		InParam.SaveObjHeader->SaveSlotNameStr = USaveLoadSystemConfig::GetSlotName(InParam);
	}

	// 将Obj序列化；根据【保存类型】选择不同的序列化方式。
	FSaveSystemSavingFeature SaveFeature;
	SaveFeature.FromSaveType(InParam.SaveType);
	if(SaveFeature.bUseOfficial)
	{
		bSerializedSuccess = UGameplayStatics::SaveGameToMemory(InParam.MainSaveObj, ObjectBytes);
	}
	else
	{
		bSerializedSuccess = UDongSaveSystemStatic_Simple::SaveGameToMemory(InParam.MainSaveObj, InParam.SaveObjHeader, ObjectBytes);
	}

	// 序列化成功，则将结果保存到文件。
	if (bSerializedSuccess)
	{
		return FFileHelper::SaveArrayToFile(ObjectBytes, *SaveFullPath);
	}

	return false;
}

USaveGame* UDongSaveSystemStatic_Simple::LoadGameFromSlot(FDongSaveSystemSavingParam& InOutParam)
{
	TArray<uint8> ObjectBytes;

	// 检查 传入的InParam 是否合法，保存时，故需要设定默认参数。
	if (!USaveLoadSystemConfig::CheckSaveSystemSavingParam(InOutParam, true))
	{
		return nullptr;
	}
	
	FString&& SaveFullPath = GetPath(InOutParam);

	// 从文件读取序列化的对象
	if (FFileHelper::LoadFileToArray(ObjectBytes, *SaveFullPath))
	{
		// 从文件读取成功的话则根据【保存类型】，来决定如何 反序列化
		FSaveSystemSavingFeature SaveFeature;
		SaveFeature.FromSaveType(InOutParam.SaveType);
		if(SaveFeature.bUseOfficial)
		{
			return UGameplayStatics::LoadGameFromMemory(ObjectBytes);
		}
		else
		{
			return UDongSaveSystemStatic_Simple::LoadGameFromMemory(ObjectBytes, InOutParam);
		}
	}
	return nullptr;
}

bool UDongSaveSystemStatic_Simple::DoesSaveGameExist(FDongSaveSystemSavingParam& InParam)
{
	FString&& SaveFullPath = GetPath(InParam);
	if (IFileManager::Get().FileSize(*SaveFullPath) >= 0)
	{
		return true;
	}
	return false;
}

bool UDongSaveSystemStatic_Simple::DeleteGameInSlot(FDongSaveSystemSavingParam& InParam)
{
	FString&& SaveFullPath = GetPath(InParam);
	return IFileManager::Get().Delete(*SaveFullPath, true, false, true);
}

bool UDongSaveSystemStatic_Simple::SaveActorToMemory(AActor* InSaveActor, TArray<uint8>& OutSaveData)
{
	if (InSaveActor)
	{
		FMemoryWriter MemWriter(OutSaveData, true);
		FSaveGameActorArchiveProxy ActorWriter(MemWriter);
		ActorWriter << InSaveActor;

		return true; // Not sure if there's a failure case here.
	}

	return false;
}

AActor* UDongSaveSystemStatic_Simple::LoadActorFromMemory(const TArray<uint8>& InSaveData, AActor* InOutLoadActor)
{
	if (InSaveData.Num() == 0)
	{
		// Empty buffer, return instead of causing a bad serialize that could crash
		return nullptr;
	}

	FMemoryReader MemReader(InSaveData, true);
	FSaveGameActorArchiveProxy ActorWriter(MemReader);
	ActorWriter << InOutLoadActor;

	return InOutLoadActor;
}

void UDongSaveSystemStatic_Simple::AsyncSaveGameToSlot(FDongSaveSystemSavingParam& InParam, FAsyncSaveGameToSlotDelegate SavedDelegate)
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [InParam, SavedDelegate]()
	{
		FDongSaveSystemSavingParam CopiedParam = InParam;
		const bool bSuccess = UDongSaveSystemStatic_Simple::SaveGameToSlot(CopiedParam);
		// Now schedule the callback on the game thread, but only if it was bound to anything
		if (SavedDelegate.IsBound())
		{
			FString&& SaveFullPath = GetPath(CopiedParam);
			SavedDelegate.ExecuteIfBound(SaveFullPath, INDEX_NONE, bSuccess);
		}
	});
}

void UDongSaveSystemStatic_Simple::AsyncLoadGameFromSlot(FDongSaveSystemSavingParam& InParam, FOnAsyncLoadFromSaveArea LoadedDelegate)
{
	AsyncTask(ENamedThreads::GameThread, [InParam, LoadedDelegate]()
	{
		FDongSaveSystemSavingParam CopiedParam = InParam;
		LoadGameFromSlot(CopiedParam);
		FString&& SaveFullPath = GetPath(CopiedParam);
		LoadedDelegate.ExecuteIfBound(SaveFullPath, INDEX_NONE, &CopiedParam);
	});
}
