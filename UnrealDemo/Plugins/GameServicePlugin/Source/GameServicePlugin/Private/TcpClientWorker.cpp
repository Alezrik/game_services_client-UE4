// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientWorker.h"
#include "PlatformProcess.h"
#include "TcpClientSender.h"
#include "TcpCommandProcessor.h"
#include "RunnableThread.h"

TcpClientWorker::TcpClientWorker(TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket)
{
	this->Socket = Socket;
	this->Serializer = NewObject<UBinarySerializer>();
	this->DeSerializer = NewObject<UBinaryDeSerializer>();
	this->ClientSenderPtr = new TcpClientSender(this->Socket);
	this->ClientSenderPtr->TcpClient_OnSendData.AddRaw(this, &TcpClientWorker::OnTcpClientData);
	this->CommandProcessorPtr = new TcpCommandProcessor(ClientSenderPtr);
	this->ClientAuthentication = NewObject<UTcpClientAuthentication>();
	this->ClientAuthentication->Initialize(this->ClientSenderPtr);
	this->CommandProcessorPtr->OnReceiveServerAuthenticateChallenge.AddUObject(this->ClientAuthentication, &UTcpClientAuthentication::OnSmsgAuthenticateChallenge);
	this->CommandProcessorPtr->OnReceiveServerAuthenticate.AddUObject(this->ClientAuthentication, &UTcpClientAuthentication::OnSmsgAuthenticate);
}

bool TcpClientWorker::Init()
{
	return true;
}
uint32 TcpClientWorker::Run()
{
	ExecuteLoop = true;
	FDateTime StartTIme = FDateTime::Now();
	SendThread = FRunnableThread::Create(ClientSenderPtr, TEXT("TcpClientSender"));
	CmdProcessThread = FRunnableThread::Create(CommandProcessorPtr, TEXT("TcpCommandProcessor"));
	LastActivity = FDateTime::Now();
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
			if (HeartBeatCheck.GetTotalSeconds() > 12 && ClientSenderPtr->HasMessagesQueued() == false)
			{
				//get game server heartbeat message
				TArray<uint8> HeartBeatData = Serializer->GetCmsgHeartbeatMessage();
				this->ClientSenderPtr->SendMessage(HeartBeatData);
			}

			TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
			if(SocketPtr.IsValid())
			{
				
				
				bool hasData = SocketPtr->HasPendingData(DataAvailable);
				if (hasData)
				{
					SocketPtr->Recv(DataIn, DataAvailable, BytesRead);
					TArray<uint8> ReceivedData;
					for(int x =0;x<BytesRead;x++)
					{
						ReceivedData.Add(DataIn[x]);
						
					}
					CommandProcessorPtr->ProcessBytes(ReceivedData);
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

UTcpClientAuthentication* TcpClientWorker::GetClientAuthentication()
{
	return ClientAuthentication;
}

void TcpClientWorker::OnTcpClientData(int32 BytesSent)
{
	LastActivity = FDateTime::Now();
}


