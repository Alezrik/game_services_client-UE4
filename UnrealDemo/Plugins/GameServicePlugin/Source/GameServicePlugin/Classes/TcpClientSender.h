// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runnable.h"
#include "Sockets.h"
#include "Queue.h"

/**
 * 
 */
class GAMESERVICEPLUGIN_API TcpClientSender :  public FRunnable
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FTcpClientSendData, int32);

	TcpClientSender(TWeakPtr<FSocket, ESPMode::ThreadSafe> socket);;
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	UFUNCTION()
	void FlushAndComplete();;
	UFUNCTION()
	void SendMessage(TArray<uint8> message);
	UFUNCTION()
	bool HasMessagesQueued();
	FTcpClientSendData TcpClient_OnSendData;

private:
	UPROPERTY()
	TQueue<TArray<uint8>, EQueueMode::Spsc> SendMessageQueue;
	UPROPERTY()
	bool ExecuteLoop = false;
	UPROPERTY()
	TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket;
};
