#pragma once
UENUM(BlueprintType)
enum class ETcpClientAuthenticationStatus : uint8 {
	AUTHSTATUS_NOT_INITALIZED UMETA(DisplayName = "Not Initialized"),
	AUTHSTATUS_INITIALIZED UMETA(DisplayName="Initialized"),
	AUTHSTATUS_CHALLENGE_SENT UMETA(DisplayName = "Challenge Sent"),
	AUTHSTATUS_CHALLENGE_RECEIVED UMETA(DisplayName = "Challenge Received"),
	AUTHSTATUS_CREDENTIAL_SENT UMETA(DisplayName = "Credentials Sent"),
	AUTHSTATUS_AUTHENTICATED UMETA(DisplayName = "Authenticated"),
	AUTHSTATUS_AUTH_ERROR UMETA (DisplayName = "Authentication Error")
};
