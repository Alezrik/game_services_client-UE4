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
	TcpClientSender(TWeakPtr<FSocket> socket) { this->Socket = socket; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	void FlushAndComplete() { ExecuteLoop = false; };
	void SendMessage(TArray<uint8> message) { SendMessageQueue.Enqueue(message); };

private:
	TQueue<TArray<uint8>, EQueueMode::Spsc> SendMessageQueue;

	bool ExecuteLoop = false;
	TWeakPtr<FSocket> Socket;
};
