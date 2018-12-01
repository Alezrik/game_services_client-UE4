// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClient.h"
#include "IPv4Address.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

ETcpClientStatus UTcpClient::InitializeClient(TSharedPtr<FSocket, ESPMode::ThreadSafe> ConfiguredSocket)
{
	if(ConfiguredSocket.IsValid())
	{
		Socket = ConfiguredSocket;
		if(Socket.IsValid())
		{
			TcpClientWorker = new ::TcpClientWorker(Socket.Pin());

		}		
	}
	return GetClientStatus();
}

ETcpClientStatus UTcpClient::ShutdownClient()
{
	if (Socket.IsValid())
	{
		Socket.Reset();
		Socket = nullptr;
	}
	if(TcpClientWorker)
	{
		delete TcpClientWorker;
		TcpClientWorker = nullptr;
	}
	return GetClientStatus();
}

EGameServiceConnectionStatus UTcpClient::ConnectToGameService()
{
	if(Socket.IsValid())
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if(SocketPtr.IsValid())
		{
			if(SocketPtr->GetConnectionState() != ESocketConnectionState::SCS_Connected)
			{
				bool success = SocketPtr->Connect(*GetGameServiceConnectionAddress());
				if (success)
				{
					TcpClientWorkerThread = FRunnableThread::Create(TcpClientWorker, TEXT("TcpClientWorker"));
				}
			}
			
		}
	}
	return GetGameServiceConnectionStatus();
}

EGameServiceConnectionStatus UTcpClient::DisconnectFromGameService()
{
	if(TcpClientWorkerThread)
	{
		TcpClientWorker->FlushAndComplete();
		TcpClientWorkerThread->WaitForCompletion();
		delete TcpClientWorkerThread;
		TcpClientWorkerThread = nullptr;
	}
	if(Socket.IsValid())
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if(SocketPtr.IsValid())
		{
			if (SocketPtr->GetConnectionState() == ESocketConnectionState::SCS_Connected)
			{
				bool success = SocketPtr->Close();
				UE_LOG(LogTemp, Warning, TEXT("Closed Socket Success is %s"), success? TEXT("true"):TEXT("false"));
				
			}
		}
		
	}
	return GetGameServiceConnectionStatus();
}

UTcpClientAuthentication* UTcpClient::GetAuthentication()
{
	return this->TcpClientWorker->GetClientAuthentication();
}


ETcpClientStatus UTcpClient::GetClientStatus()
{
	if(Socket.IsValid() && TcpClientWorker)
	{
		if(TcpClientWorkerThread)
		{
		
			return ETcpClientStatus::CCLIENT_RUNNING;
			
		}
		else
		{
			return ETcpClientStatus::CCLINET_STOPPED;
		}
	}
	else
	{
		return ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
	}	
}

EGameServiceConnectionStatus UTcpClient::GetGameServiceConnectionStatus()
{
	if(Socket.IsValid() && TcpClientWorker)
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if(SocketPtr.IsValid())
		{
			switch(SocketPtr->GetConnectionState())
			{
			case ESocketConnectionState::SCS_Connected:
				return EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER;
			case ESocketConnectionState::SCS_NotConnected:
				return EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
			case ESocketConnectionState::SCS_ConnectionError:
				return EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR;
			default:
				return EGameServiceConnectionStatus::CSTATUS_UNKNOWN;
			}
		}
		else
		{
			return EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
		}
	}
	else
	{
		return EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
	}
}
TSharedRef<FInternetAddr> UTcpClient::GetGameServiceConnectionAddress()
{
	FIPv4Address ip(127, 0, 0, 1);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(8005);
	return addr;
}




