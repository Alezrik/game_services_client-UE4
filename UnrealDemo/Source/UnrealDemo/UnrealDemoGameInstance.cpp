// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealDemoGameInstance.h"


#include "SocketSubsystem.h"

#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include <memory>




void UUnrealDemoGameInstance::Init()
{
	GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
	TcpClient = MakeShareable(NewObject<UTcpClient>());
}
void UUnrealDemoGameInstance::Shutdown()
{
	if(TcpClient.IsValid())
	{
		TcpClient->StopWorker();
		TcpClient->ShutdownClient();
		TcpClient.Reset();
	}
	
}



TSharedRef<FInternetAddr> UUnrealDemoGameInstance::GetGameServiceConnectionAddress()
{
	FIPv4Address ip(127, 0, 0, 1);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(8005);
	return addr;
}

EGameServiceConnectionStatus UUnrealDemoGameInstance::ConnectToGameService()
{
	Socket = MakeShareable(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false));
	Socket->SetNonBlocking(true);
	if(GameServiceConnectionStatus != EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED)
	{
		return GameServiceConnectionStatus;
	}

	bool connected = Socket->Connect(*GetGameServiceConnectionAddress());
	if (connected == true)
	{
		GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER;
		ETcpClientStatus clients_status = TcpClient->InitializeClient(Socket);
		if(clients_status == ETcpClientStatus::CCLIENT_INITIALIZED)
		{
			clients_status = TcpClient->StartWorker();
			if(clients_status!=ETcpClientStatus::CCLIENT_RUNNING)
			{
				TcpClient->ShutdownClient();
				Socket.Reset();
				GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR;
			}
		}
		else
		{
			Socket.Reset();
			GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR;
		}
	}
	else
	{
		GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR;
	}
	return GameServiceConnectionStatus;
	
}

EGameServiceConnectionStatus UUnrealDemoGameInstance::ResetConnectionError()
{
	if (GameServiceConnectionStatus != EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR)
		return GameServiceConnectionStatus;
	GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
	return GameServiceConnectionStatus;
}

EGameServiceConnectionStatus UUnrealDemoGameInstance::CloseConnection()
{
	if (GameServiceConnectionStatus == EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER)
	{
		TcpClient->StopWorker();
		TcpClient->ShutdownClient();
		if (Socket.Get() != nullptr && Socket.IsValid() == true && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			
			Socket->Close();

			Socket.Reset();
			GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
		}
	}
	return GameServiceConnectionStatus;
	
}








