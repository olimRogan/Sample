// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OlimDownloadImage.generated.h"

class UTexture2DDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOlimDownloadImageDelegate, FString, Url, UTexture2DDynamic*, Texture, FVector2D, Size);

UCLASS()
class OLIMPLUGIN_API UOlimDownloadImage : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UOlimDownloadImage* DownloadImage(FString URL);

public:

	//UPROPERTY(BlueprintAssignable)
	//	FDownloadImageDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FOlimDownloadImageDelegate OnSuccess2;

	UPROPERTY(BlueprintAssignable)
		FOlimDownloadImageDelegate OnFail2;

public:
	FString Url;
	void Start(FString URL);

private:

	/** Handles image requests coming from the web */
	void HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void HandleImageRequest(TArray<uint8> data);
};
