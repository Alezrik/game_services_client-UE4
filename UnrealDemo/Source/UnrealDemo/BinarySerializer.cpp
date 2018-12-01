// Fill out your copyright notice in the Description page of Project Settings.

#include "BinarySerializer.h"
#include "Base64.h"

TArray<uint8> UBinarySerializer::GetCmsgHeartbeatMessage()
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

TArray<uint8> UBinarySerializer::GetGmsgHeartbeatMessage()
{
	TArray<uint8> ArrayTestMessage;
	uint8* Header = this->UInt64ToBinary(102	);
	uint8 MessageContents[1] = { 1 };
	uint8* MessageSize = this->ReverseEldian(this->UInt32ToBinary(sizeof(MessageContents)), 4);
	for (int x = 0; x < 8; x++)
	{
		ArrayTestMessage.Add(Header[x]);
	}
	for (int x = 0; x < 4; x++)
	{
		ArrayTestMessage.Add(MessageSize[x]);
	}
	ArrayTestMessage.Add(MessageContents[0]);
	delete Header;
	delete MessageSize;
	return ArrayTestMessage;
}

TArray<uint8> UBinarySerializer::GetCmsgAuthenticateChallenge(FString userName, uint32 ChallengeKey)
{
	TArray<uint8> CmsgAuthenticateChallenge;
	uint8* Header = this->UInt64ToBinary(1);
	TCHAR* NameTchar = userName.GetCharArray().GetData();
	uint8* NameBin =(uint8*)(TCHAR_TO_UTF8(NameTchar));
	uint8 NameLen = FCString::Strlen(NameTchar);

	uint8* ChallengeBin = UInt32ToBinary(ChallengeKey);

	TArray<uint8> Message;
	Message.Add(NameLen);
	for(int x=0;x<NameLen;x++)
	{
		Message.Add(NameBin[x]);
	}
	for(int x=0;x<4;x++)
	{
		Message.Add(ChallengeBin[x]);
	}

	for(int x =0;x<8;x++)
	{
		CmsgAuthenticateChallenge.Add(Header[x]);
	}
	uint8* MessageLen = ReverseEldian(UInt32ToBinary(Message.Num()), 4);
	for(int x =0;x<4;x++)
	{
		CmsgAuthenticateChallenge.Add(MessageLen[x]);
	}
	for(int x=0;x<Message.Num();x++)
	{
		CmsgAuthenticateChallenge.Add(Message[x]);
	}
	return CmsgAuthenticateChallenge;

}

TArray<uint8> UBinarySerializer::GetCmsgAuthenticate(FString password, FString salt, uint32 ClientKey, uint32 ServerKey)
{
	TArray<uint8> Message;
	uint8* Header = this->UInt64ToBinary(4);
	TArray<uint8> MessageContents;

	FString MessageStr = FString::FromInt(ClientKey) + "+" + FString::FromInt(ServerKey) + "+" + password + "+" + salt;
	FString MessageStrEnc = FBase64::Encode(MessageStr);
	uint8* MessageStrEncBin = (uint8*)TCHAR_TO_UTF8(MessageStrEnc.GetCharArray().GetData());
	int MessageLen = MessageStrEnc.Len();
	for(int x =0;x<MessageStrEnc.Len();x++)
	{
		MessageContents.Add(MessageStrEnc[x]);
	}
	uint8* MessageContentLen = ReverseEldian(UInt32ToBinary(MessageContents.Num()), 4);
	for(int x=0;x<8;x++)
	{
		Message.Add(Header[x]);
	}
	for(int x=0;x<4;x++)
	{
		Message.Add(MessageContentLen[x]);
	}
	for(int x =0;x<MessageContents.Num();x++)
	{
		Message.Add(MessageContents[x]);
	}
	return Message;
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



