// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TcpClientStatus.h"
#include "Sockets.h"
#include "MultichannelTcpReceiver.h"
#include "TcpClientWorker.h"
#include "TcpClient.generated.h"


/**
 * 
 */
UCLASS()
class UNREALDEMO_API UTcpClient : public UObject
{
	GENERATED_BODY()

public:
	UTcpClient(const FObjectInitializer & ObjectInitializer);
	ETcpClientStatus GetClientStatus() { return ClientStatus; };
	ETcpClientStatus InitializeClient(TSharedPtr<FSocket> Socket);
	ETcpClientStatus ShutdownClient();
	ETcpClientStatus StartWorker();
	ETcpClientStatus StopWorker();
private:
	ETcpClientStatus ClientStatus;
	TWeakPtr<FSocket> Socket;
	TcpClientWorker* TcpClientWorker = nullptr;
	FRunnableThread* Thread = nullptr;
	
};
