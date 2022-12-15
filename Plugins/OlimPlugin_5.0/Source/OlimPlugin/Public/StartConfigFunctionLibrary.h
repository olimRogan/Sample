// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseConfig.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StartConfigFunctionLibrary.generated.h"

/**
 * 
 */

class ContentsStartConfig : public BaseConfig
{
public:
	ContentsStartConfig(void) : BaseConfig("ContentsStartConfig.json")
	{
	}

private:
	bool TotalAutoMode = false;
	//bool FullHmdMode = false;
	bool ExteriorMenuNotShow = false;
	bool InteriorMenuNotShow = false;
	bool EstateMenuNotShow = false;
	bool MapMenuNotShow = false;
	bool ConditionMenuNotShow = false;
	bool GuideBookMenuNotShow = false;

public:
	virtual TSharedPtr<FJsonObject> CreateJsonObject(void) override
	{
		TSharedPtr<FJsonObject> jDefault = MakeShareable(new FJsonObject);

		jDefault->SetBoolField("TotalAutoMode", TotalAutoMode);
		//jDefault->SetBoolField("FullHmdMode", FullHmdMode);
		jDefault->SetBoolField("ExteriorMenuNotShow", ExteriorMenuNotShow);
		jDefault->SetBoolField("InteriorMenuNotShow", InteriorMenuNotShow);
		jDefault->SetBoolField("EstateMenuNotShow", EstateMenuNotShow);
		jDefault->SetBoolField("MapMenuNotShow", MapMenuNotShow);
		jDefault->SetBoolField("ConditionMenuNotShow", ConditionMenuNotShow);
		jDefault->SetBoolField("GuideBookMenuNotShow", GuideBookMenuNotShow);

		return jDefault;
	}
};


UCLASS()
class OLIMPLUGIN_API UStartConfigFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	
	
public:
	UStartConfigFunctionLibrary();

	UFUNCTION(BlueprintCallable, Category = "Start Config")
	static void GetStartConfig(const FString& Key, bool& Boolean, int& Integer, float& Float, FString& String);
	static void LoadConfig();
};
