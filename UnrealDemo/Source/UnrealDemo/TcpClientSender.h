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
class UNREALDEMO_API TcpClientSender :  public FRunnable
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FTcpClientSendData, int32);

	TcpClientSender(TWeakPtr<FSocket, ESPMode::ThreadSafe> socket);;
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	void FlushAndComplete();;
	void SendMessage(TArray<uint8> message);
	bool HasMessagesQueued();
	FTcpClientSendData TcpClient_OnSendData;

private:
	TQueue<TArray<uint8>, EQueueMode::Spsc> SendMessageQueue;
	bool ExecuteLoop = false;
	TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket;
};
