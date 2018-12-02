// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "TcpClientTest.generated.h"


/**
 * Example UStruct declared in a plugin module
 */
USTRUCT()
struct FTcpClientTestStruct
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY()
	FString TestString;
};
 

/**
 * Example of declaring a UObject in a plugin module
 */
UCLASS()
class GAMESERVICEPLUGIN_API UTcpClientTest: public UObject
{
	GENERATED_BODY()

public:

private:

	UPROPERTY()
		FTcpClientTestStruct MyStruct;

};


