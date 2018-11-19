#pragma once
UENUM(BlueprintType)
enum class EGameServiceConnectionStatus: uint8{
	CSTATUS_CONNECTION_ERROR UMETA(DisplayName = "Connection Error"),
	CSTATUS_NOT_INITALIZED UMETA(DisplayName =  "Not Initialized"),
	CSTATUS_NOT_CONNECTED UMETA(DisplayName = "Not Connected"),
	CSTATUS_CONNECTED_TO_SERVER UMETA(DisplayName = "Connected to Server")

};


