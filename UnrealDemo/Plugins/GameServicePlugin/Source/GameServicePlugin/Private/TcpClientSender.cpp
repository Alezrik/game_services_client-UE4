// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientSender.h"
#include "SocketSubsystem.h"


TcpClientSender::TcpClientSender(TWeakPtr<FSocket, ESPMode::ThreadSafe> socket)
{
	this->Socket = socket;
}

bool TcpClientSender::Init()
{
	return true;
}

void TcpClientSender::Stop()
{
	ExecuteLoop = false;
}

void TcpClientSender::FlushAndComplete()
{
	ExecuteLoop = false;
}

void TcpClientSender::SendMessage(TArray<uint8> message)
{
	SendMessageQueue.Enqueue(message);
}

bool TcpClientSender::HasMessagesQueued()
{
	return !(SendMessageQueue.IsEmpty());
}


uint32 TcpClientSender::Run()
{
	FDateTime StartTIme = FDateTime::Now();
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
					TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
					if(SocketPtr.IsValid())
					{
						bool success = SocketPtr->Send(Message.GetData(), Message.Num(), BytesSent);
						if(success)
						{
							if(TcpClient_OnSendData.IsBound())
							{
								TcpClient_OnSendData.Broadcast(BytesSent);
							}
							
						}
						
					}
					
				}
			}
		}
		
	}
	Socket = nullptr;
	return 0;
}
