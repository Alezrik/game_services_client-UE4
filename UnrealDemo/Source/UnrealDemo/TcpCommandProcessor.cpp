// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpCommandProcessor.h"

TcpCommandProcessor::TcpCommandProcessor(TcpClientSender* sender)
{
	Sender = sender;
}

bool TcpCommandProcessor::Init()
{
	return true;
}

void TcpCommandProcessor::Stop()
{
	ExecuteLoop = false;
}

uint32 TcpCommandProcessor::Run()
{
	return 0;
}