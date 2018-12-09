// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealDemoGameInstance.h"


#include "SocketSubsystem.h"

#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include <memory>




void UUnrealDemoGameInstance::Init()
{
	//SetupTcpClient();
}
void UUnrealDemoGameInstance::Shutdown()
{
	//TeardownTcpClient();
}

void UUnrealDemoGameInstance::SetupTcpClient()
{
	Socket = MakeShareable(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false));
	TcpClient = NewObject<UTcpClient>();
	TcpClient->InitializeClient(Socket);
	TcpClientTest = NewObject<UTcpClientTest>();
}

void UUnrealDemoGameInstance::TeardownTcpClient()
{
	if(TcpClient)
	{
		TcpClient->ShutdownClient();
		TcpClient = nullptr;
	}
	if(Socket.IsValid())
	{
		Socket.Reset();
		Socket = nullptr;
	}
	
	
}

UTcpClient* UUnrealDemoGameInstance::GetTcpClient()
{
	if (!TcpClient)
	{
		SetupTcpClient();
		TcpClient->ConnectToGameService();
	}
	return TcpClient;
}










