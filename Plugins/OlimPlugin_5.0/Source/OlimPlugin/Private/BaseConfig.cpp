// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseConfig.h"
#include "OlimBlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/Paths.h"

void BaseConfig::Init()
{
	if (Load() == false)
		CreateDefault();
}

void BaseConfig::CreateDefault(void)
{
	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	FJsonSerializer::Serialize(CreateJsonObject().ToSharedRef(), JsonWriter);
	UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../Config/", FileName);
}

bool BaseConfig::Load(void)
{
	FString ConfigRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../Config/", FileName);

	if (ConfigRaw.IsEmpty())
		return false;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ConfigRaw);
	if (FJsonSerializer::Deserialize(JsonReader, CurrentConfig) == false)
		return false;

	auto DefaultJson = CreateJsonObject();
	if (CurrentConfig->Values.Num() == DefaultJson->Values.Num())
		return true;

	for (auto Value : DefaultJson->Values)
	{
		if (CurrentConfig->HasField(Value.Key) == false)
			CurrentConfig->SetField(Value.Key, Value.Value);
	}

	SaveCurrentConfig();
	return true;
}

bool BaseConfig::SaveCurrentConfig(void)
{
	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	FJsonSerializer::Serialize(CurrentConfig.ToSharedRef(), JsonWriter);
	UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../Config/", FileName);
	return false;
}

int BaseConfig::GetInteger(FString Key)
{
	if (CurrentConfig == nullptr)
		Load();
	int32 Value = 0;
	if (CurrentConfig->TryGetNumberField(Key, Value))
		return Value;
	return Value;
}

float BaseConfig::GetFloat(FString Key)
{
	if (CurrentConfig == nullptr)
		Load();
	double Value = 0.f;
	if (CurrentConfig->TryGetNumberField(Key, Value))
		return Value;
	return static_cast<float>(Value);
}

FString BaseConfig::GetString(FString Key)
{
	if (CurrentConfig == nullptr)
		Load();
	FString Value;
	if (CurrentConfig->TryGetStringField(Key, Value))
		return Value;
	return Value;
}

bool BaseConfig::GetBoolean(FString Key)
{
	if (CurrentConfig == nullptr)
		Load();
	bool Value = false;
	if (CurrentConfig->TryGetBoolField(Key, Value))
		return Value;
	return Value;
}

void BaseConfig::SetValue(FString Key, bool Value)
{
	CurrentConfig->SetBoolField(Key, Value);
}

void BaseConfig::SetValue(FString Key, int Value)
{
	CurrentConfig->SetNumberField(Key, Value);
}

void BaseConfig::SetValue(FString Key, float Value)
{
	CurrentConfig->SetNumberField(Key, Value);
}

void BaseConfig::SetValue(FString Key, FString Value)
{
	CurrentConfig->SetStringField(Key, Value);
}

