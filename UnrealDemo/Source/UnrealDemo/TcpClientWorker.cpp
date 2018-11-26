// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientWorker.h"
#include "PlatformProcess.h"
#include "TcpClientSender.h"
#include "TcpCommandProcessor.h"
#include "RunnableThread.h"

bool TcpClientWorker::Init()
{
	return true;
}
uint32 TcpClientWorker::Run()
{
	ExecuteLoop = true;
	FDateTime StartTIme = FDateTime::Now();
	FDateTime LastActivity = FDateTime::Now();
	SendThread = FRunnableThread::Create(ClientSenderPtr, TEXT("TcpClientSender"));
	CmdProcessThread = FRunnableThread::Create(CommandProcessorPtr, TEXT("TcpCommandProcessor"));
	while(ExecuteLoop)
	{
		uint8 DataIn[1024];
		// read will 0 out when finished with data...
		int32 BytesRead = 0;
		uint32 DataAvailable = 0;
		uint32 BufferSize = 1024;
		
		FTimespan HeartBeatCheck = FDateTime::Now() - LastActivity;
		if(Socket.IsValid())
		{
			if (HeartBeatCheck.GetTotalSeconds() > 4)
			{
				TArray<uint8> HeartBeatData = Serializer->GetCClinetHeartbeatMessage();
				this->ClientSenderPtr->SendMessage(HeartBeatData);
				LastActivity = FDateTime::Now();
			}

			TSharedPtr<FSocket> SocketPtr = Socket.Pin();
			if(SocketPtr.IsValid())
			{
				
				
				bool hasData = SocketPtr->HasPendingData(DataAvailable);
				if (hasData)
				{
					LastActivity = FDateTime::Now();
					SocketPtr->Recv(DataIn, DataAvailable, BytesRead);
					UE_LOG(LogTemp, Warning, TEXT("Socket Receive: %d"), DataAvailable);
					TArray<uint8> ReceivedData;
					for(int x =0;x<BytesRead;x++)
					{
						ReceivedData.Add(DataIn[x]);
						ReceiveMessageQueue.Enqueue(ReceivedData);
					}
				}	
			}
		}
		
	}
	CommandProcessorPtr->FlushAndComplete();
	ClientSenderPtr->FlushAndComplete();
	CmdProcessThread->WaitForCompletion();
	SendThread->WaitForCompletion();
	Socket = nullptr;
	return 0;
}

void TcpClientWorker::Stop()
{
	ExecuteLoop = false;


}

