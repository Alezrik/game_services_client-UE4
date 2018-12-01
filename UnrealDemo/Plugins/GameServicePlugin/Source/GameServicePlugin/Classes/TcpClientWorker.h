// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Queue.h"
#include "CoreMinimal.h"
#include "Sockets.h"
#include "BinarySerializer.h"
#include "BinaryDeSerializer.h"
#include "TcpCommandProcessor.h"
#include "TcpClientSender.h"
#include "TcpClientAuthentication.h"

/**
 * 
 */
class GAMESERVICEPLUGIN_API TcpClientWorker : public FRunnable
{
public:
	TcpClientWorker(TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket);
	UFUNCTION()
	void FlushAndComplete() { ExecuteLoop = false; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	
	UFUNCTION()
	UTcpClientAuthentication* GetClientAuthentication();

	UFUNCTION()
	void OnTcpClientData(int32 BytesSent);

private:
	UPROPERTY()
	bool ExecuteLoop = false;
	UPROPERTY()
	TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket;
	UPROPERTY()
	UBinarySerializer* Serializer;
	UPROPERTY()
	UBinaryDeSerializer* DeSerializer;
	UPROPERTY()
	FRunnableThread* SendThread = nullptr;
	UPROPERTY()
	FRunnableThread* CmdProcessThread = nullptr;
	UPROPERTY()
	TcpCommandProcessor* CommandProcessorPtr = nullptr;
	UPROPERTY()
	TcpClientSender* ClientSenderPtr = nullptr;
	UPROPERTY()
	FDateTime LastActivity;
	UPROPERTY()
	UTcpClientAuthentication* ClientAuthentication = nullptr;
	
};
