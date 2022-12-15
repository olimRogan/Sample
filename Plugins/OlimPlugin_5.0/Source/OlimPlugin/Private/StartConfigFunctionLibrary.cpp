// Fill out your copyright notice in the Description page of Project Settings.


#include "StartConfigFunctionLibrary.h"

TSharedPtr<BaseConfig> _ContentsStartConfig;

UStartConfigFunctionLibrary::UStartConfigFunctionLibrary()
{
	if (_ContentsStartConfig == nullptr)
	{
		_ContentsStartConfig = MakeShared<ContentsStartConfig>();
		_ContentsStartConfig->Init();
	}
}

void UStartConfigFunctionLibrary::GetStartConfig(const FString& Key, bool& Boolean, int& Integer, float& Float, FString& String)
{
	Boolean = _ContentsStartConfig->GetBoolean(Key);
	Integer = _ContentsStartConfig->GetInteger(Key);
	Float = _ContentsStartConfig->GetFloat(Key);
	String = _ContentsStartConfig->GetString(Key);
}

void UStartConfigFunctionLibrary::LoadConfig()
{
	_ContentsStartConfig->Load();
}
