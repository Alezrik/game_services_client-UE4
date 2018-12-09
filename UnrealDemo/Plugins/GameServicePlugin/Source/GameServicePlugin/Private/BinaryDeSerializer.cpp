// Fill out your copyright notice in the Description page of Project Settings.

#include "BinaryDeSerializer.h"
#include "IGameServicePlugin.h"



FGameServiceMessage UBinaryDeSerializer::DeserializeBinary(TArray<uint8> Binary)
{
	FGameServiceMessage GameServiceMessage;
	uint8 Header[8];
	for (int x = 0; x < 8; x++)
	{
		Header[x] = Binary[x];
	}
	uint8 MessageSizeBin[4];
	for (int x = 0; x < 4; x++)
	{
		MessageSizeBin[3 - x] = Binary[x + 8];
	}
	uint32 MessageSize = BinaryToUint32(MessageSizeBin);
	uint64 Command_ID = BinaryToUint64(Header);
	TArray<uint8> Contents;
	for(uint32 x =0;x<MessageSize;x++)
	{
		Contents.Add(Binary[x + 12]);
	}

	GameServiceMessage.CommandId = Command_ID;
	GameServiceMessage.MessageLength = MessageSize;
	GameServiceMessage.MessageContents = ParseMessageContents(Command_ID, Contents);
	return GameServiceMessage;
}

TMap<FString, FString> UBinaryDeSerializer::ParseMessageContents(uint64 Command_ID, TArray<uint8> MessageContents)
{
	switch(Command_ID)
	{
	case 2:
		return ParseSmsgAuthenticateChallenge(MessageContents);
	case 5:
		return ParseSmsgAuthenticate(MessageContents);
	case 101:
		//SMSG_HEARTBEAT
		return ParseSmsgHeartbeat(MessageContents);
	default:
		return TMap<FString, FString>();
	}
	
}

uint64 UBinaryDeSerializer::BinaryToUint64(uint8* var)
{
	uint32 lowest_pos = 0;

	return  (((uint64)var[lowest_pos]) << 56) |
		(((uint64)var[lowest_pos + 1]) << 48) |
		(((uint64)var[lowest_pos + 2]) << 40) |
		(((uint64)var[lowest_pos + 3]) << 32) |
		(((uint64)var[lowest_pos + 4]) << 24) |
		(((uint64)var[lowest_pos + 5]) << 16) |
		(((uint64)var[lowest_pos + 6]) << 8) |
		(((uint64)var[lowest_pos + 7]) << 0);

}

uint32 UBinaryDeSerializer::BinaryToUint32(uint8* var)
{
	uint32 lowest_pos = 0;

	return
		(((uint64)var[lowest_pos]) << 24) |
		(((uint64)var[lowest_pos + 1]) << 16) |
		(((uint64)var[lowest_pos + 2]) << 8) |
		(((uint64)var[lowest_pos + 3]) << 0);

}

TMap<FString, FString> UBinaryDeSerializer::ParseSmsgHeartbeat(TArray<uint8> MessageContents)
{
	TMap<FString, FString> Message;
	uint8 MessageValueInt = MessageContents[0];
	FString MessageValue = FString::FromInt(MessageValueInt);
	Message.Add("HeartbeatMessage",  MessageValue);
	return Message;
}
TMap<FString, FString> UBinaryDeSerializer::ParseSmsgAuthenticateChallenge(TArray<uint8> MessageContents)
{
	TMap<FString, FString> Message;
	Message.Empty();
	uint8 ServerRandBin[4];
	for(int x =0;x<4;x++)
	{
		ServerRandBin[x] = MessageContents[x];
	}
	uint32 ServerRand = BinaryToUint32(ServerRandBin);
	uint8 SaltLenBin[4];
	for(int x=0;x<4;x++)
	{
		SaltLenBin[x] = MessageContents[x + 4];
	}
	uint32 SaltLen = BinaryToUint32(SaltLenBin);
	TArray<uint8> SaltBin;
	for(uint32 x=0;x<SaltLen;x++)
	{
		int offset = 8;
		SaltBin.Add(MessageContents[x + offset]);
	}
	FString Salt = UTF8_TO_TCHAR(SaltBin.GetData());
	UE_LOG(GameServiceLog, Warning, TEXT("SaltLen: %d, Salt: %s"), SaltLen, *Salt);
	Message.Add("ServerKey", FString::FromInt(ServerRand));
	Message.Add("Salt", Salt);
	return Message;


	
}

TMap<FString, FString> UBinaryDeSerializer::ParseSmsgAuthenticate(TArray<uint8> MessageContents)
{
	TMap<FString, FString> Message;
	if(MessageContents[0] == 1)
	{
		Message.Add("success", "true");
		uint8 TokenLenBin[4];
		for (int x = 0; x < 4; x++)
		{
			TokenLenBin[x] = MessageContents[x + 1];
		}
		uint32 TokenLen = BinaryToUint32(TokenLenBin);
		TArray<uint8> TokenBin;
		for(uint32 x = 0;x<TokenLen;x++)
		{
			TokenBin.Add(MessageContents[x + 5]);
		}
		Message.Add("token", UTF8_TO_TCHAR(TokenBin.GetData()));
	}
	else
	{
		Message.Add("success", "false");
	}
	
	return Message;
}
