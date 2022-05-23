// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * @brief 
 * 
 */
struct BASEUI_MODULE_API FMemoryWriter_UObject : public FMemoryWriter
{
	FMemoryWriter_UObject(TArray<uint8>& InBytes, bool bIsPersistent, bool bSetOffset = false, const FName InArchiveName = NAME_None )
	: FMemoryWriter(InBytes, bIsPersistent, bSetOffset, InArchiveName) {}

	TArray<uint8>& GetData() { return Bytes; }

	// virtual FArchive& operator<<( class UObject*& Res ) override
	// {
	// 	if (IsValid(Res))
	// 	{
	// 		// FObjectAndNameAsStringProxyArchive
	// 		Res->Serialize(*this);
	// 	}
	// 	return *this;
	// }
};

/**
 * @brief 
 * 
 */
struct BASEUI_MODULE_API FMemoryReader_UObject : public FMemoryReader
{
	FMemoryReader_UObject(const TArray<uint8>& InBytes, bool bIsPersistent)
	: FMemoryReader(InBytes, bIsPersistent) { BytesPtr = &InBytes; }

	const TArray<uint8>* BytesPtr;
};
