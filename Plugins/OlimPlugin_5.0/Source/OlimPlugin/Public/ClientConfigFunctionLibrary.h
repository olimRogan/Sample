// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseConfig.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClientConfigFunctionLibrary.generated.h"

/**
 * 
 */



class ClientConfig : public BaseConfig
{
public:
	ClientConfig(void) : BaseConfig("ClientConfig.json")
	{

	}
private:
	// general
	float Volume = 1.f;
	float AutoModeSpeed = 5.f;
	int ResolutionNum = 0; // 0, 1, 2;
	FString Language = "ko";
	// control
	bool IsTouchInterface = false;
	bool MouseDirectionReverse = false;
	float CameraHeight = 50.f;
	float CameraRotationSpeed = 5.f;
	// menubar
	int MenubarTB = 0; // 0, 1
	int MenubarWN = 0; // 0, 1
	int MenubarLCR = 0; // 0, 1, 2
	// submenu
	int SubMenuRL = 0; // 0, 1

public:
	virtual TSharedPtr<FJsonObject> CreateJsonObject(void) override
	{
		TSharedPtr<FJsonObject> jDefault = MakeShareable(new FJsonObject);

		//float Volume = 100.f;
		jDefault->SetNumberField("Volume", Volume);
		//float AutoModeSpeed = 10.f;
		jDefault->SetNumberField("AutoModeSpeed", AutoModeSpeed);
		//int ResolutionNum = 2; // 0, 1, 2;
		jDefault->SetNumberField("ResolutionNum", ResolutionNum);
		//FString Language = "ko";
		jDefault->SetStringField("Language", Language);
		//bool IsTouchInterface = true;
		jDefault->SetBoolField("IsTouchInterface", IsTouchInterface);
		//bool MouseDirectionReverse = false;
		jDefault->SetBoolField("MouseDirectionReverse", MouseDirectionReverse);
		//float CameraHeight = 10.f;
		jDefault->SetNumberField("CameraHeight", CameraHeight);
		//float CameraRotationSpeed = 10.f;
		jDefault->SetNumberField("CameraRotationSpeed", CameraRotationSpeed);
		//int MenubarTB = 0; // 0, 1
		jDefault->SetNumberField("MenubarTB", MenubarTB);
		//int MenubarWN = 0; // 0, 1
		jDefault->SetNumberField("MenubarWN", MenubarWN);
		//int MenubarLCR = 0; // 0, 1, 2
		jDefault->SetNumberField("MenubarLCR", MenubarLCR);
		//int SubMenuRL = 0; // 0, 1
		jDefault->SetNumberField("SubMenuRL", SubMenuRL);

		return jDefault;
	}
};

UCLASS()
class OLIMPLUGIN_API UClientConfigFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UClientConfigFunctionLibrary();
public:
	UFUNCTION(BlueprintCallable, Category = "ClientConfig")
	static void GetClientConfig(const FString& Key, bool& Boolean, int& Integer, float& Float, FString& String);
	UFUNCTION(BlueprintCallable, Category = "ClientConfig")
	static void SetClientConfigBoolean(const FString& Key, bool Value);
	UFUNCTION(BlueprintCallable, Category = "ClientConfig")
	static void SetClientConfigInteger(const FString& Key, int Value);
	UFUNCTION(BlueprintCallable, Category = "ClientConfig")
	static void SetClientConfigFloat(const FString& Key, float Value);
	UFUNCTION(BlueprintCallable, Category = "ClientConfig")
	static void SetClientConfigString(const FString& Key, FString Value);
};
