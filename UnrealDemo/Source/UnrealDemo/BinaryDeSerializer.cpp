// Fill out your copyright notice in the Description page of Project Settings.

#include "BinaryDeSerializer.h"

FGameServiceMessage UBinaryDeSerializer::DeserializeBinary(TArray<uint8> Binary)
{
	FGameServiceMessage GameServiceMessage;
	uint8 Header[8];
	for (int x = 0; x < 8; x++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Byte: %d is: %d"), x, Binary[x]);
		Header[x] = Binary[x];
	}
	uint8 MessageSizeBin[4];
	for (int x = 0; x < 4; x++)
	{
		MessageSizeBin[3 - x] = Binary[x + 8];
	}
	uint32 MessageSize = BinaryToUint32(MessageSizeBin);
	uint64 Command_ID = BinaryToUint64(Header);
	UE_LOG(LogTemp, Warning, TEXT("Received Cmd: %d with size: %d"), Command_ID, MessageSize);
	TArray<uint8> Contents;
	for(uint32 x =0;x<MessageSize;x++)
	{
		Contents.Add(Binary[x + 12]);
	}

	GameServiceMessage.CommandId = Command_ID;
	GameServiceMessage.MessageLength = MessageSize;
	GameServiceMessage.MessageContents = Contents;
	return GameServiceMessage;
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
