// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TcpClientStatus.h"
#include "Sockets.h"
#include "MultichannelTcpReceiver.h"
#include "TcpClientWorker.h"
#include "UnrealDemoConnectionStatus.h"
#include "TcpClientManager.h"
#include "TcpClient.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class GAMESERVICEPLUGIN_API UTcpClient : public UObject
{
	GENERATED_BODY()

public:
	
	ETcpClientStatus InitializeClient(TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket);
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	ETcpClientStatus ShutdownClient();
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	EGameServiceConnectionStatus ConnectToGameService();
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	EGameServiceConnectionStatus DisconnectFromGameService();

	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	UTcpClientAuthentication* GetAuthentication();
	UFUNCTION(BlueprintCallable, Category = "GameServices-TcpClient")
	UTcpClientManager* GetClientManager();


private:
	TSharedRef<FInternetAddr> GetGameServiceConnectionAddress();
	TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket;
	TcpClientWorker* TcpClientWorker = nullptr;
	FRunnableThread* TcpClientWorkerThread = nullptr;
	UTcpClientManager* TcpClientManager = nullptr;
	
};
