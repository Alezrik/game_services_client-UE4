// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealDemoConnectionStatus.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"
#include "MultichannelTcpReceiver.h"
#include "MultichannelTcpSender.h"
#include "TcpClient.h"
#include "TcpClientTest.h"
#include "SocketSubsystem.h"
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
	virtual void Shutdown() override;
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	void SetupTcpClient();
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	void TeardownTcpClient();
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	UTcpClient* GetTcpClient();

private:
	TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket;
	UPROPERTY()
	UTcpClient* TcpClient = nullptr;
	UTcpClientTest* TcpClientTest = nullptr;
};
