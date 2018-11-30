// Fill out your copyright notice in the Description page of Project Settings.

#include "BinarySerializer.h"

TArray<uint8> UBinarySerializer::GetCClinetHeartbeatMessage()
{
	TArray<uint8> ArrayTestMessage;
	uint8* Header = this->UInt64ToBinary(100);
	uint8 MessageContents[1] = { 1 };
	uint8* MessageSize = this->ReverseEldian(this->UInt32ToBinary(sizeof(MessageContents)), 4);
	for(int x =0;x<8;x++)
	{
		ArrayTestMessage.Add(Header[x]);
	}
	for(int x=0;x<4;x++)
	{
		ArrayTestMessage.Add(MessageSize[x]);
	}
	ArrayTestMessage.Add(MessageContents[0]);
	delete Header;
	delete MessageSize;
	return ArrayTestMessage;
}

uint8* UBinarySerializer::ReverseEldian(uint8* binary, int length)
{
	uint8* Reverse = new uint8[length];
	for(int x =0;x<length;x++)
	{
		Reverse[x] = binary[(length - 1) - x];
	}
	delete binary;
	return Reverse;
}

uint8* UBinarySerializer::UInt32ToBinary(uint32 integer)
{
	uint8* Binary = new uint8[4];
	Binary[0] = (integer >> 24) & 0xFF;
	Binary[1] = (integer >> 16) & 0xFF;
	Binary[2] = (integer >> 8) & 0xFF;
	Binary[3] = integer & 0xFF;
	return Binary;
}

uint8* UBinarySerializer::UInt64ToBinary(uint64 integer)
{
	uint8* Binary = new uint8[8];
	Binary[0] = (integer >> 56) & 0xFF;
	Binary[1] = (integer >> 48) & 0xFF;
	Binary[2] = (integer >> 40) & 0xFF;
	Binary[3] = (integer >> 32) & 0xFF;
	Binary[4] = (integer >> 24) & 0xFF;
	Binary[5] = (integer >> 16) & 0xFF;
	Binary[6] = (integer >> 8) & 0xFF;
	Binary[7] = integer & 0xFF;
	return Binary;
}



