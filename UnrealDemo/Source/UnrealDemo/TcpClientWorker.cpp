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
		if(!SendMessageQueue.IsEmpty())
		{
			while(!SendMessageQueue.IsEmpty())
			{
				TArray<uint8> Message;
				SendMessageQueue.Dequeue(Message);
				
			}
		}
		if(HeartBeatCheck.GetTotalSeconds() > 1)
		{
			uint8* HeartBeatData = Serializer->GetCClinetHeartbeatMessage();
			int32 BytesSent;
			
			Socket->Send(HeartBeatData, 13, BytesSent);
			LastActivity = FDateTime::Now();
			UE_LOG(LogTemp, Warning, TEXT("Heart Beat: %d, datasize: %d"), BytesSent, sizeof(HeartBeatData));
		}
		bool hasData = Socket->HasPendingData(DataAvailable);
		if (hasData)
		{
			LastActivity = FDateTime::Now();
			Socket->Recv(DataIn, DataAvailable, BytesRead);
			UE_LOG(LogTemp, Warning, TEXT("Socket Receive: %d"), DataAvailable);
		}
	}
	Socket.Reset();
	return 0;
}

void TcpClientWorker::Stop()
{
	ExecuteLoop = false;
}

void TcpClientWorker::SendMessage(TArray<uint8> Message)
{
	SendMessageQueue.Enqueue(Message);
}

