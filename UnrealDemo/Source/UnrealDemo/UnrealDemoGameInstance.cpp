// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealDemoGameInstance.h"


#include "SocketSubsystem.h"

#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"


void UUnrealDemoGameInstance::Init()
{
	this->Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
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

void UUnrealDemoGameInstance::ConnectToGameService()
{
	if(GameServiceConnectionStatus != EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED)
	{
		throw FString("Invalid Status for Connection");
	}

	bool connected = Socket->Connect(*GetGameServiceConnectionAddress());
	if(connected == true)
	{
		GameServiceConnectionStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER;
	}
}

