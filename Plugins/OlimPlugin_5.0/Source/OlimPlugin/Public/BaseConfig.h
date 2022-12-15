// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Json.h"

/**
 *
 */
class OLIMPLUGIN_API BaseConfig
{
public:
	BaseConfig(FString _FileName) : FileName(_FileName)
	{

	}
	virtual ~BaseConfig() {};
public:
	virtual TSharedPtr<FJsonObject> CreateJsonObject(void) = 0;
	void Init();

public:
	void CreateDefault(void);
	bool Load(void);
	bool SaveCurrentConfig(void);

public:
	int GetInteger(FString Key);
	float GetFloat(FString Key);
	FString GetString(FString Key);
	bool GetBoolean(FString Key);

public:
	void SetValue(FString Key, bool Value);
	void SetValue(FString Key, int Value);
	void SetValue(FString Key, float Value);
	void SetValue(FString Key, FString Value);

private:
	TSharedPtr<FJsonObject> CurrentConfig = nullptr;
	FString FileName;
};
