// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClient.h"
#include "IPv4Address.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

ETcpClientStatus UTcpClient::InitializeClient(TSharedPtr<FSocket, ESPMode::ThreadSafe> ConfiguredSocket)
{
	if(ConfiguredSocket.IsValid())
	{
		Socket = ConfiguredSocket;
		if(Socket.IsValid())
		{
			TcpClientWorker = new ::TcpClientWorker(Socket.Pin());

		}		
	}
	TcpClientManager = NewObject<UTcpClientManager>();
	TcpClientManager->RefreshStatus(Socket, TcpClientWorker, TcpClientWorkerThread);
	return TcpClientManager->GetClientStatus();
}

ETcpClientStatus UTcpClient::ShutdownClient()
{
	if (Socket.IsValid())
	{
		Socket.Reset();
		Socket = nullptr;
	}
	if(TcpClientWorker)
	{
		delete TcpClientWorker;
		TcpClientWorker = nullptr;
	}
	TcpClientManager->RefreshStatus(Socket, TcpClientWorker, TcpClientWorkerThread);
	return TcpClientManager->GetClientStatus();
}

EGameServiceConnectionStatus UTcpClient::ConnectToGameService()
{
	if(Socket.IsValid())
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if(SocketPtr.IsValid())
		{
			if(SocketPtr->GetConnectionState() != ESocketConnectionState::SCS_Connected)
			{
				bool success = SocketPtr->Connect(*GetGameServiceConnectionAddress());
				if (success)
				{
					TcpClientWorkerThread = FRunnableThread::Create(TcpClientWorker, TEXT("TcpClientWorker"));
				}
			}
			
		}
	}
	TcpClientManager->RefreshStatus(Socket, TcpClientWorker, TcpClientWorkerThread);
	return TcpClientManager->GetConnectionStatus();
}

EGameServiceConnectionStatus UTcpClient::DisconnectFromGameService()
{
	if(TcpClientWorkerThread)
	{
		TcpClientWorker->FlushAndComplete();
		TcpClientWorkerThread->WaitForCompletion();
		delete TcpClientWorkerThread;
		TcpClientWorkerThread = nullptr;
	}
	if(Socket.IsValid())
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if(SocketPtr.IsValid())
		{
			if (SocketPtr->GetConnectionState() == ESocketConnectionState::SCS_Connected)
			{
				bool success = SocketPtr->Close();
				UE_LOG(LogTemp, Warning, TEXT("Closed Socket Success is %s"), success? TEXT("true"):TEXT("false"));
				
			}
		}
		
	}
	TcpClientManager->RefreshStatus(Socket, TcpClientWorker, TcpClientWorkerThread);
	return TcpClientManager->GetConnectionStatus();
}

UTcpClientAuthentication* UTcpClient::GetAuthentication()
{
	return this->TcpClientWorker->GetClientAuthentication();
}

UTcpClientManager* UTcpClient::GetClientManager()
{
	return this->TcpClientManager;
}


TSharedRef<FInternetAddr> UTcpClient::GetGameServiceConnectionAddress()
{
	FIPv4Address ip(127, 0, 0, 1);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(8005);
	return addr;
}




