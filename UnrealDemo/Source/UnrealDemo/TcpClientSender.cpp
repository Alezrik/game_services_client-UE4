// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientSender.h"

bool TcpClientSender::Init()
{
	return true;
}

void TcpClientSender::Stop()
{
	ExecuteLoop = false;
}


uint32 TcpClientSender::Run()
{
	FDateTime StartTIme = FDateTime::Now();
	FDateTime LastActivity = FDateTime::Now();
	ExecuteLoop = true;
	while(ExecuteLoop)
	{
		if (!SendMessageQueue.IsEmpty())
		{
			while (!SendMessageQueue.IsEmpty())
			{
				if (Socket.IsValid())
				{
					int32 BytesSent;
					TArray<uint8> Message;
					SendMessageQueue.Dequeue(Message);
					TSharedPtr<FSocket> SocketPtr = Socket.Pin();
					SocketPtr->Send(Message.GetData(), Message.Num(), BytesSent);
					LastActivity = FDateTime::Now();
					UE_LOG(LogTemp, Warning, TEXT("Heart Beat: %d, datasize: %d"), BytesSent, Message.Num());
				}
			}
		}
	}
	return 0;
}