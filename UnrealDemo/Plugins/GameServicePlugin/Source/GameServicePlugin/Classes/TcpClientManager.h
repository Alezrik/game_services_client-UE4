// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TcpClientStatus.h"
#include "Sockets.h"
#include "MultichannelTcpReceiver.h"
#include "TcpClientWorker.h"
#include "UnrealDemoConnectionStatus.h"
#include "TcpClientManager.generated.h"


/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class GAMESERVICEPLUGIN_API UTcpClientManager : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpClientStatusChange, ETcpClientStatus, Status);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpConnectionStatusChange, EGameServiceConnectionStatus, Status);

public:
	UPROPERTY(BlueprintAssignable, Category = "GameServices-Status")
	FTcpClientStatusChange OnClientStatusChange;
	UPROPERTY(BlueprintAssignable, Category = "GameServices-Status")
	FTcpConnectionStatusChange OnConnectionStatusChange;
	void RefreshStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker, FRunnableThread* TcpClientWorkerThread);
	UFUNCTION(BlueprintCallable, Category = "GameServices-Status")
	ETcpClientStatus GetClientStatus();
	UFUNCTION(BlueprintCallable, Category = "GameServices-Status")
	EGameServiceConnectionStatus GetConnectionStatus();
private:
	ETcpClientStatus ClientStatus = ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
	EGameServiceConnectionStatus ConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
	ETcpClientStatus RefreshClientStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker, FRunnableThread* TcpClientWorkerThread);
	EGameServiceConnectionStatus RefreshConnectionStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker);

};