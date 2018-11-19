// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealDemoGameInstance.h"


#include "SocketSubsystem.h"

#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include <memory>




void UUnrealDemoGameInstance::Init()
{
	GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
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
	if(GameServiceConnectionStatus != EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED)
	{
		return GameServiceConnectionStatus;
	}

	bool connected = Socket->Connect(*GetGameServiceConnectionAddress());
	if (connected == true)
	{
		GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER;
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
		Socket->Close();
		Socket.Reset();
		GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
	}
	return GameServiceConnectionStatus;
	
}



