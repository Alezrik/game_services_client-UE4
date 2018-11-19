// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientWorker.h"
#include "PlatformProcess.h"

bool TcpClientWorker::Init()
{
	return true;
}
uint32 TcpClientWorker::Run()
{
	ExecuteLoop = true;
	FDateTime StartTIme = FDateTime::Now();
	FDateTime LastActivity = FDateTime::Now();
	while(ExecuteLoop == true)
	{
		uint8 DataIn[1024];
		// read will 0 out when finished with data...
		int32 BytesRead = 0;
		uint32 DataAvailable = 0;
		uint32 BufferSize = 1024;
		
		FTimespan HeartBeatCheck = FDateTime::Now() - LastActivity;
		if(HeartBeatCheck.GetTotalSeconds() > 1)
		{
			uint8 HeartBeatData[14] = {0,0,0,0,0,0,0,66,2,0,0,0,1,2};
			int32 BytesSent;
			Socket->Send(HeartBeatData, 14, BytesSent);
			LastActivity = FDateTime::Now();
			UE_LOG(LogTemp, Warning, TEXT("Heart Beat"));
		}
		bool hasData = Socket->HasPendingData(DataAvailable);
		if (BytesRead > 0)
		{
			LastActivity = FDateTime::Now();
			Socket->Recv(DataIn, BufferSize, BytesRead);
		}
	}
	Socket.Reset();
	return 0;
}

void TcpClientWorker::Stop()
{
	ExecuteLoop = false;
}

