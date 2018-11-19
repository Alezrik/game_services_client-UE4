#pragma once
UENUM(BlueprintType)
enum class ETcpClientStatus: uint8 {
	CCLIENT_NOT_INITIALIZED  UMETA(DisplayName = "Client Not Initialized"),
	CCLIENT_INITIALIZED UMETA(DisplayName = "Client Initialized"),
	CCLIENT_RUNNING UMETA(DisplayName = "Client Running"),
	CCLINET_STOPPED UMETA(DisplayName = "Client Stopped")
};


