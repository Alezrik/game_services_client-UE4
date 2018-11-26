// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClient.h"

UTcpClient::UTcpClient(const FObjectInitializer& ObjectInitializer)
{
	ClientStatus = ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
}

ETcpClientStatus UTcpClient::InitializeClient(TSharedPtr<FSocket> Socket)
{
	if(Socket.IsValid() && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected && ClientStatus == ETcpClientStatus::CCLIENT_NOT_INITIALIZED)
	{
		this->Socket = Socket;
		TcpClientWorker = new ::TcpClientWorker(Socket);
		ClientStatus = ETcpClientStatus::CCLIENT_INITIALIZED;
	}
	return ClientStatus;
}

ETcpClientStatus UTcpClient::ShutdownClient()
{
	if (Socket.Get() != nullptr && Socket.IsValid() && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		if(ClientStatus == ETcpClientStatus::CCLIENT_INITIALIZED || ClientStatus == ETcpClientStatus::CCLINET_STOPPED)
		{
			Socket.Reset();
			ClientStatus = ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
			delete TcpClientWorker;
			TcpClientWorker = nullptr;

		}
	}
	return ClientStatus;
}

ETcpClientStatus UTcpClient::StartWorker()
{
	if(Socket.Get() != nullptr && Socket.IsValid() && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected && Thread == nullptr)
	{
		if(ClientStatus == ETcpClientStatus::CCLIENT_INITIALIZED || ClientStatus == ETcpClientStatus::CCLINET_STOPPED)
		{
			Thread = FRunnableThread::Create(TcpClientWorker, TEXT("TcpClientWorker"));
			ClientStatus = ETcpClientStatus::CCLIENT_RUNNING;
		}
		
	}
	return ClientStatus;
	
}

ETcpClientStatus UTcpClient::StopWorker()
{
	if(ClientStatus == ETcpClientStatus::CCLIENT_RUNNING)
	{
		TcpClientWorker->FlushAndComplete();
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
		ClientStatus = ETcpClientStatus::CCLINET_STOPPED;
	}
	return ClientStatus;
}
