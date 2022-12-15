// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EngineMinimal.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "OlimWebConvertFunction.generated.h"


UCLASS()
class OLIMPLUGIN_API UOlimWebConvertFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
		
		//Inteior
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool JsonInfoFileCheck(const FString& mapName, FString& fileName);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool InteriorArrInfoGen(const FString& fileName, const FString& levelName, const FString& TypeBtnName, const TArray<FString>& minimapName, 
			const TArray<ATargetPoint*> targetGath, const TArray<FVector2D>& scaleCorr, const TArray<FVector2D>& posCorr, const bool& isMultiFloor, const bool& isReverse, const int& ins2ndStartNum);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool Interior360ArrInfoGen(const FString& fileName, const FString& levelName, const FString& TypeBtnName, const TArray<FString>& minimapName, 
			const TArray<FString>& imageName360, const TArray<FVector2D>& minimapPoint, const TArray<float>& direction, const TArray<float>& angle);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool InteriorBtnInfoGen(const FString& fileName, const int& length, const TArray<FString>& mapNameArr, const TArray<FString>& btnImageNameArr, 
			const TArray<FString>& btn3dModeArr, const TArray<FString>& DpName);

		//Iso
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool IsoArrInfoGen(const FString& fileName, const FString& levelName, const int& AddAngle);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool IsoWebImgGen(const FString& fileName, const FString& levelName, const int& index);

		//Exterior
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool ExteriorArrInfoGen(const FString& fileName, const FString& levelName, const TArray<UTexture*> imageArr, const TArray<FRotator>& defaltRot, 
			const TArray<float>& arrowAngle, const TArray<int>& iconNum, const TArray<int>& iconInfo, const TArray<FString>& iconName, const TArray<FVector>& iconPos);
		
		//Area
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool AreaArrInfoGen(const FString& fileName, const FString& Area360LevelName, const FString& Area3DLevelName, const TArray<FString>& AreaSelectorBgName,
			const TArray<float>& PosX, const TArray<float>& PosY, const TArray<FString>& Type, const TArray<FString>& Src, const TArray<int>& PageNum);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool Area3DArrInfoGen(const FString& fileName, const FString& levelName, const FString minimapName, const TArray<ATargetPoint*> targetGath, const FVector2D& scaleCorr, const FVector2D& posCorr);

		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static bool Area360ArrInfoGen(const FString& fileName, const FString& levelName, const FString& minimapName, const TArray<FString>& imageName360, const TArray<float>& direction, const TArray<float>& angle,
			const TArray<FVector2D>& minimapPos, const TArray<FString>& tag, const TArray<int>& selfNum, const TArray<int>& TargetNum, const TArray<FVector>& targetPos);


		//instrument
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static FVector2D PosToRad(const FVector StartPos, const FVector EndPos);
		
		UFUNCTION(BlueprintCallable, Category = "WebConvertFunction")
		static float DegreeToRad(const float degree);

};
