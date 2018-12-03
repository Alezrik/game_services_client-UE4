// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClientAuthentication.h"
#include "BinarySerializer.h"

void UTcpClientAuthentication::Initialize(TcpClientSender* sender)
{
	ClientSender = sender;
	AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_INITIALIZED;
}

int UTcpClientAuthentication::SendClientAuthenticateChallenge(FString userName)
{
	uint32 client_rand = FMath::RandRange(0, 99999999);
	UBinarySerializer* serializer = NewObject<UBinarySerializer>();
	TArray<uint8> message = serializer->GetCmsgAuthenticateChallenge(userName, client_rand);
	ClientSender->SendMessage(message);
	AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT;
	ClientKey = client_rand;
	if(AuthenticationStatus!= ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT)
	{
		if (OnAuthStatusChange.IsBound())
			OnAuthStatusChange.Broadcast(AuthenticationStatus);
	}
	
	return client_rand;
}

void UTcpClientAuthentication::SendClientAuthenticate(FString password)
{
	UBinarySerializer* serializer = NewObject<UBinarySerializer>();
	TArray<uint8> message = serializer->GetCmsgAuthenticate(password, Salt, ClientKey, ServerKey);
	ClientSender->SendMessage(message);
	AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_CREDENTIAL_SENT;
	if (AuthenticationStatus != ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT)
	{
		if (OnAuthStatusChange.IsBound())
			OnAuthStatusChange.Broadcast(AuthenticationStatus);
	}
	
}

int UTcpClientAuthentication::GetClientAuthkey()
{
	return ClientKey;
}

int UTcpClientAuthentication::GetServerAuthkey()
{
	return ServerKey;
}

FString UTcpClientAuthentication::GetSalt()
{
	return Salt;
}

ETcpClientAuthenticationStatus UTcpClientAuthentication::GetAuthenticationStatus()
{
	return AuthenticationStatus;
}

void UTcpClientAuthentication::OnSmsgAuthenticateChallenge(FGameServiceMessage Message)
{
	ServerKey = FCString::Atoi(Message.MessageContents.Find("ServerKey")->GetCharArray().GetData());
	Salt = (Message.MessageContents.Find("Salt")->GetCharArray().GetData());
	AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_RECEIVED;
	if (AuthenticationStatus != ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT)
	{
		if (OnAuthStatusChange.IsBound())
			OnAuthStatusChange.Broadcast(AuthenticationStatus);
	}
	
}

void UTcpClientAuthentication::OnSmsgAuthenticate(FGameServiceMessage Message)
{
	UE_LOG(LogTemp, Warning, TEXT("Got Message at handler"));
	if(Message.MessageContents.Find("success")->Equals("true"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Auth Success"))
		Token = Message.MessageContents.Find("token")->GetCharArray().GetData();
		AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_AUTHENTICATED;
		if (AuthenticationStatus != ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT)
		{
			if (OnAuthStatusChange.IsBound())
				OnAuthStatusChange.Broadcast(AuthenticationStatus);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Auth Failure"));
		Token = "nil";
		AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_AUTH_ERROR;
		if (AuthenticationStatus != ETcpClientAuthenticationStatus::AUTHSTATUS_CHALLENGE_SENT)
		{
			if (OnAuthStatusChange.IsBound())
				OnAuthStatusChange.Broadcast(AuthenticationStatus);
		}
		
	}
}
