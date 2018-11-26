// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BinarySerializer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDEMO_API UBinarySerializer : public UObject
{
	GENERATED_BODY()

public:
	TArray<uint8> GetCClinetHeartbeatMessage();
			
private:
	uint8* UInt32ToBinary(uint32 integer);
	uint8* UInt64ToBinary(uint64 integer);
	uint8* ReverseEldian(uint8* binary, int length);

};