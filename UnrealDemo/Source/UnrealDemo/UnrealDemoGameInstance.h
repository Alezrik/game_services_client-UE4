// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealDemoConnectionStatus.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"

#include "UnrealDemoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDEMO_API UUnrealDemoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	UFUNCTION(BlueprintCallable, Category="GameServices")
	EGameServiceConnectionStatus GetGameServiceConnectionStatus() { return GameServiceConnectionStatus; };
	UFUNCTION(BlueprintCallable, Category = "GameServices")
	void ConnectToGameService();
	

private:
	FSocket* Socket = NULL;
	TSharedRef<FInternetAddr> GetGameServiceConnectionAddress();
	EGameServiceConnectionStatus GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;

};
