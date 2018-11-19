// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runnable.h"

#include "CoreMinimal.h"
#include "Sockets.h"

/**
 * 
 */
class UNREALDEMO_API TcpClientWorker : public FRunnable
{
public:
	TcpClientWorker(TSharedPtr<FSocket> Socket) { this->Socket = Socket; };
	void FlushAndComplete() { ExecuteLoop = false; };
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	bool ExecuteLoop = false;
	TSharedPtr<FSocket> Socket;
};
