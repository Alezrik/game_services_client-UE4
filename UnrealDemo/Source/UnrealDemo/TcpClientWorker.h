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
	TcpClientWorker(TSharedPtr<FSocket> Socket)
	{
		this->Socket = Socket;
		this->Serializer = NewObject<UBinarySerializer>();
		this->DeSerializer = NewObject<UBinaryDeSerializer>();
		this->ClientSenderPtr = new TcpClientSender(this->Socket);
		this->CommandProcessorPtr = new TcpCommandProcessor();

	};
	void FlushAndComplete() { ExecuteLoop = false; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	//void SendMessage(TArray<uint8> Message);

private:
	bool ExecuteLoop = false;
	TWeakPtr<FSocket> Socket;
	TQueue<TArray<uint8>, EQueueMode::Spsc> ReceiveMessageQueue;
	UBinarySerializer* Serializer;
	UBinaryDeSerializer* DeSerializer;
	FRunnableThread* SendThread = nullptr;
	FRunnableThread* CmdProcessThread = nullptr;

	TcpCommandProcessor* CommandProcessorPtr = nullptr;
	TcpClientSender* ClientSenderPtr = nullptr;
};
