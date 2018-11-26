// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runnable.h"
#include "Queue.h"
#include "CoreMinimal.h"
#include "Sockets.h"
#include "BinarySerializer.h"
#include "BinaryDeSerializer.h"

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

	};
	void FlushAndComplete() { ExecuteLoop = false; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void SendMessage(TArray<uint8> Message);

private:
	bool ExecuteLoop = false;
	TSharedPtr<FSocket> Socket;
	TQueue<TArray<uint8>, EQueueMode::Spsc> SendMessageQueue;
	UBinarySerializer* Serializer;
	UBinaryDeSerializer* DeSerializer;
};
