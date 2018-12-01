// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpCommandProcessor.h"

TcpCommandProcessor::TcpCommandProcessor(TcpClientSender* sender)
{
	Sender = sender;
	DeSerializer = NewObject<UBinaryDeSerializer>();
}

bool TcpCommandProcessor::Init()
{
	return true;
}

void TcpCommandProcessor::Stop()
{
	ExecuteLoop = false;
}

uint32 TcpCommandProcessor::Run()
{
	ExecuteLoop = true;
	TArray<uint8> UnparsedBinary;
	while (ExecuteLoop == true)
	{
		if(!ReceiveMessageQueue.IsEmpty() || UnparsedBinary.Num() > 0)
		{
			TArray<uint8> BytesToProcess;
			if(!ReceiveMessageQueue.IsEmpty())
				ReceiveMessageQueue.Dequeue(BytesToProcess);
			if (UnparsedBinary.Num() > 0)
			{
				for(int x = 0;x<BytesToProcess.Num();x++)
				{
					UnparsedBinary.Add(BytesToProcess[x]);
				}
				BytesToProcess = UnparsedBinary;
				UnparsedBinary.Empty();
			}
			int32 EndMessageByte = BytesToProcess.Find('\0');
			FGameServiceMessage GameServiceMessage = DeSerializer->DeserializeBinary(BytesToProcess);
			switch(GameServiceMessage.CommandId)
			{
			case 2:
				UE_LOG(LogTemp, Warning, TEXT("Received SMSG Auth Challenge Msg"));
				if (OnReceiveServerAuthenticateChallenge.IsBound())
				{
					OnReceiveServerAuthenticateChallenge.Broadcast(GameServiceMessage);
				}
				break;

			case 5:
				UE_LOG(LogTemp, Warning, TEXT("Received SMSG Auth Msg"));
				if(OnReceiveServerAuthenticate.IsBound())
				{
					OnReceiveServerAuthenticate.Broadcast(GameServiceMessage);
				}
				break;
			}
			
			if(GameServiceMessage.MessageLength + 12 != BytesToProcess.Num())
			{
				for(int x = GameServiceMessage.MessageLength + 12;x<BytesToProcess.Num();x++)
				{
					UnparsedBinary.Add(BytesToProcess[x]);
				}
			}
			
		}
		
	}
	return 0;
}

void TcpCommandProcessor::FlushAndComplete()
{
	ExecuteLoop = false;
}

void TcpCommandProcessor::ProcessBytes(TArray<uint8> MessageBytes)
{
	ReceiveMessageQueue.Enqueue(MessageBytes);
}

