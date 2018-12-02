#include "TcpClientManager.h"

void UTcpClientManager::RefreshStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker,
	FRunnableThread* TcpClientWorkerThread)
{
	RefreshConnectionStatus(Socket, TcpClientWorker);
	RefreshClientStatus(Socket, TcpClientWorker, TcpClientWorkerThread);
	
}


ETcpClientStatus UTcpClientManager::GetClientStatus()
{
	return ClientStatus;
}

EGameServiceConnectionStatus UTcpClientManager::GetConnectionStatus()
{
	return ConnectionStatus;
}

ETcpClientStatus UTcpClientManager::RefreshClientStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker, FRunnableThread* TcpClientWorkerThread)
{
	ETcpClientStatus NewStatus = ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
	if (Socket.IsValid() && TcpClientWorker)
	{
		if (TcpClientWorkerThread)
		{


			NewStatus = ETcpClientStatus::CCLIENT_RUNNING;

		}
		else
		{
			NewStatus = ETcpClientStatus::CCLINET_STOPPED;
		}
	}
	else
	{
		NewStatus = ETcpClientStatus::CCLIENT_NOT_INITIALIZED;
	}
	if (NewStatus != ClientStatus )
	{
		if(OnClientStatusChange.IsBound())
			OnClientStatusChange.Broadcast(NewStatus);
	}
	ClientStatus = NewStatus;
	return ClientStatus;
}

EGameServiceConnectionStatus UTcpClientManager::RefreshConnectionStatus(TWeakPtr<FSocket, ESPMode::ThreadSafe> Socket, TcpClientWorker* TcpClientWorker)
{
	EGameServiceConnectionStatus NewStatus = EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
	if (Socket.IsValid() && TcpClientWorker)
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> SocketPtr = Socket.Pin();
		if (SocketPtr.IsValid())
		{
			switch (SocketPtr->GetConnectionState())
			{
			case ESocketConnectionState::SCS_Connected:
				NewStatus =  EGameServiceConnectionStatus::CSTATUS_CONNECTED_TO_SERVER;
				break;
			case ESocketConnectionState::SCS_NotConnected:
				NewStatus = EGameServiceConnectionStatus::CSTATUS_NOT_CONNECTED;
				break;
			case ESocketConnectionState::SCS_ConnectionError:
				NewStatus = EGameServiceConnectionStatus::CSTATUS_CONNECTION_ERROR;
				break;
			default:
				NewStatus = EGameServiceConnectionStatus::CSTATUS_UNKNOWN;
			}
		}
		else
		{
			NewStatus = EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
		}
	}
	else
	{
		NewStatus = EGameServiceConnectionStatus::CSTATUS_NOT_INITALIZED;
	}
	if (NewStatus != ConnectionStatus)
	{
		if(OnConnectionStatusChange.IsBound())
			OnConnectionStatusChange.Broadcast(NewStatus);
	}
		
	ConnectionStatus = NewStatus;
	return ConnectionStatus;
}
