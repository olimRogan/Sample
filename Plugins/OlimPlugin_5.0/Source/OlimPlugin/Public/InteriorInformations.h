// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteriorInformations.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FProductionState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") FString MapName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int MapAllNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int MapReleaseNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") bool isReleaseMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgNormalNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgNormalPtr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgHoverNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgHoverPtr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgInfoNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgInfoPtr;
};

USTRUCT(BlueprintType)
struct FInteriorSelectImage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") FString LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int LevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgNormalPtr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgHoverPtr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgInfoPtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") bool noRelease;
};

class OLIMPLUGIN_API InteriorInformations
{
public:

	FString ZipViewID;
	TArray<FProductionState> arrProductionState;
	TArray<FInteriorSelectImage> arrInteriorSelectImg;

public:
	InteriorInformations();
	InteriorInformations(const FString & zipviewID);
	~InteriorInformations();

	FString LoadProjectConfig(const FString & config);
	void SetProductionState();
	void SetInteriorSelectImages();
	void SetLevelInformations();

	TArray<UTexture2D*> DynamicLoadTextureWithKeyword(const FString & PathFromContentFolder, const FString & Keyword);
	FString ExtractionStringByKeyword(FString inSorce, const FString & prefix, const FString & suffix);


};
