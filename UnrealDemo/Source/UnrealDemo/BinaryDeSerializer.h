// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameServiceMessage.h"
#include "BinaryDeSerializer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDEMO_API UBinaryDeSerializer : public UObject
{
public:
	GENERATED_BODY()
	UFUNCTION()
	FGameServiceMessage DeserializeBinary(TArray<uint8> Binary);

private:
	uint64 BinaryToUint64(uint8* var);
	uint32 BinaryToUint32(uint8* var);
};
