#pragma once

#include "CoreMinimal.h"
#include "GameServiceMessage.generated.h"

USTRUCT()
struct FGameServiceMessage
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 CommandId;
	UPROPERTY()
	uint32 MessageLength;
	UPROPERTY()
	TMap<FString, FString> MessageContents;
};