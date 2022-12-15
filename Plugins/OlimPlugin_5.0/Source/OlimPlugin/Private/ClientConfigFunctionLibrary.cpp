// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ClientConfigFunctionLibrary.h"

TSharedPtr<BaseConfig> _ClientConfig; 

UClientConfigFunctionLibrary::UClientConfigFunctionLibrary()
{
	if (_ClientConfig == nullptr)
	{
		_ClientConfig = MakeShared<ClientConfig>();
		_ClientConfig->Init();
	}
}

void UClientConfigFunctionLibrary::GetClientConfig(const FString& Key, bool& Boolean, int& Integer, float& Float, FString& String)
{	
	Boolean = _ClientConfig->GetBoolean(Key);
	Integer = _ClientConfig->GetInteger(Key);
	Float = _ClientConfig->GetFloat(Key);
	String = _ClientConfig->GetString(Key);
}

void UClientConfigFunctionLibrary::SetClientConfigBoolean(const FString & Key, bool Value)
{
	_ClientConfig->SetValue(Key, Value);
	_ClientConfig->SaveCurrentConfig();
}

void UClientConfigFunctionLibrary::SetClientConfigInteger(const FString & Key, int Value)
{
	_ClientConfig->SetValue(Key, Value);
	_ClientConfig->SaveCurrentConfig();
}

void UClientConfigFunctionLibrary::SetClientConfigFloat(const FString & Key, float Value)
{
	_ClientConfig->SetValue(Key, Value);
	_ClientConfig->SaveCurrentConfig();
}

void UClientConfigFunctionLibrary::SetClientConfigString(const FString & Key, FString Value)
{
	_ClientConfig->SetValue(Key, Value);
	_ClientConfig->SaveCurrentConfig();
}
