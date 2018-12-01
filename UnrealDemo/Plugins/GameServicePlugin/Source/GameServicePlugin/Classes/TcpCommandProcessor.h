// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runnable.h"
#include "Sockets.h"
#include "TcpClientSender.h"
#include "BinaryDeSerializer.h"

/**
 * 
 */
class GAMESERVICEPLUGIN_API TcpCommandProcessor : public FRunnable
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FTcpRecSmsgAuthenticateChallenge, FGameServiceMessage);
	DECLARE_MULTICAST_DELEGATE_OneParam(FTcpRecSmsgAuthenticate, FGameServiceMessage);

	TcpCommandProcessor(TcpClientSender* sender);
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	UFUNCTION()
	void FlushAndComplete();
	UFUNCTION()
	void ProcessBytes(TArray<uint8> MessageBytes);
	FTcpRecSmsgAuthenticateChallenge OnReceiveServerAuthenticateChallenge;
	FTcpRecSmsgAuthenticate OnReceiveServerAuthenticate;
private:
	UPROPERTY()
	bool ExecuteLoop = false;
	UPROPERTY()
	TcpClientSender* Sender;
	UPROPERTY()
	TQueue<TArray<uint8>, EQueueMode::Spsc> ReceiveMessageQueue;
	UFUNCTION()
	uint64 BinaryToUint64(uint8* var);
	UFUNCTION()
	uint32 BinaryToUint32(uint8* var);
	UPROPERTY()
	UBinaryDeSerializer* DeSerializer;
};
