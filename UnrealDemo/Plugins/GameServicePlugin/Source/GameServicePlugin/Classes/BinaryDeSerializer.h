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
class GAMESERVICEPLUGIN_API UBinaryDeSerializer : public UObject
{
public:
	GENERATED_BODY()
	UFUNCTION()
	FGameServiceMessage DeserializeBinary(TArray<uint8> Binary);

private:
	TMap<FString, FString> ParseMessageContents(uint64 Command_ID, TArray<uint8> MessageContents);
	uint64 BinaryToUint64(uint8* var);
	uint32 BinaryToUint32(uint8* var);

	TMap<FString, FString> ParseSmsgHeartbeat(TArray<uint8> MessageContents);
	TMap<FString, FString> ParseSmsgAuthenticateChallenge(TArray<uint8> MessageContents);
	TMap<FString, FString> ParseSmsgAuthenticate(TArray<uint8> MessageContents);
};
