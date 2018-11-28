// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Queue.h"
#include "CoreMinimal.h"
#include "Sockets.h"
#include "BinarySerializer.h"
#include "BinaryDeSerializer.h"
#include "TcpCommandProcessor.h"
#include "TcpClientSender.h"

/**
 * 
 */
class UNREALDEMO_API TcpClientWorker : public FRunnable
{
public:
	TcpClientWorker(TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket);
	void FlushAndComplete() { ExecuteLoop = false; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	
	UFUNCTION()
	void OnTcpClientData(int32 BytesSent);

private:
	bool ExecuteLoop = false;
	TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket;
	TQueue<TArray<uint8>, EQueueMode::Spsc> ReceiveMessageQueue;
	UBinarySerializer* Serializer;
	UBinaryDeSerializer* DeSerializer;
	FRunnableThread* SendThread = nullptr;
	FRunnableThread* CmdProcessThread = nullptr;
	UPROPERTY()
	TcpCommandProcessor* CommandProcessorPtr = nullptr;
	UPROPERTY()
	TcpClientSender* ClientSenderPtr = nullptr;
	FDateTime LastActivity;
	
};
