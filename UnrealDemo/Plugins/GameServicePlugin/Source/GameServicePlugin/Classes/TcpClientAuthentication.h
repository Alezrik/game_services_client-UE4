// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TcpClientSender.h"
#include "TcpClientAuthenticationStatus.h"
#include "GameServiceMessage.h"

#include "TcpClientAuthentication.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMESERVICEPLUGIN_API UTcpClientAuthentication : public UObject
{
	GENERATED_BODY()

		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthenticationStatusChange, ETcpClientAuthenticationStatus, Status);


public:
	UPROPERTY(BlueprintAssignable, Category = "Tcp-Authentication")
	FAuthenticationStatusChange OnAuthStatusChange;
	void Initialize(TcpClientSender* sender);
	UFUNCTION(BLueprintCallable, Category="Tcp-Authentication")
	int SendClientAuthenticateChallenge(FString userName);
	UFUNCTION(BlueprintCallable, Category = "Tcp-Authentication")
	void SendClientAuthenticate(FString password);
	UFUNCTION(BLueprintCallable, Category = "Tcp-Authentication")
	int GetClientAuthkey();
	UFUNCTION(BLueprintCallable, Category = "Tcp-Authentication")
	int GetServerAuthkey();
	UFUNCTION(BlueprintCallable, Category = "Tcp-Authentication")
	FString GetSalt();
	UFUNCTION(BlueprintCallable, Category = "Tcp-Authentication")
	ETcpClientAuthenticationStatus GetAuthenticationStatus();
	UFUNCTION()
	void OnSmsgAuthenticateChallenge(FGameServiceMessage Message);
	UFUNCTION()
	void OnSmsgAuthenticate(FGameServiceMessage Message);
	
private:
	ETcpClientAuthenticationStatus AuthenticationStatus = ETcpClientAuthenticationStatus::AUTHSTATUS_NOT_INITALIZED;
	TcpClientSender* ClientSender = nullptr;
	uint32 ClientKey = -1;
	uint32 ServerKey = -1;
	FString Salt;
	FString Token;

};
