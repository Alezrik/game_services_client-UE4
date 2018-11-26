// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runnable.h"
#include "Sockets.h"

/**
 * 
 */
class UNREALDEMO_API TcpCommandProcessor : public FRunnable
{
public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	void FlushAndComplete() { ExecuteLoop = false; };

private:
	bool ExecuteLoop = false;
	TWeakPtr<FSocket> Socket;
};