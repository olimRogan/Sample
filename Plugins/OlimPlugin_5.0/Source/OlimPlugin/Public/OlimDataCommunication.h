// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OlimDataCommunication.generated.h"


/*
// Delegate
UENUM(BlueprintType, Category = "Zipview Data")
enum class EDownloadResult : uint8
{
	Success,
	DownloadFailed,
	SaveFailed,
	DirectoryCreationFailed
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResult, const EDownloadResult, Result);
*/

USTRUCT(BlueprintType)
struct FProductionBasicInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") bool isResultVailed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> BasicInfoAll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> SourceType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> ImagesName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> ImagesURL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TMap<FString, FString> BasicInfoMap;
};

USTRUCT(BlueprintType)
struct FProductionTypeInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") bool isResultVailed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> TypeInfoAll;

	//UPROPERTY(BlueprintReadOnly) bool IsPublic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> SourceType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> ImagesName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> ImagesURL;
};

USTRUCT(BlueprintType)
struct FGuideBookInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") bool isResultVailed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> Subject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> WebPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Data") TArray<FString> Format;
};

UCLASS(Blueprintable)
class OLIMPLUGIN_API UOlimDataCommunication : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static void GetProductAllInfoData(const FString URL);

	static void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static FProductionBasicInfo ProductInfoFilter(FString ZipviewID);

	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static FProductionTypeInfo TypeInfoFilter(FString ZipviewID, FString TypeCode);

	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static FGuideBookInfo GuideBookFilter(FString ZipviewID);

	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static bool FileCheckAndGenerate(FString ZipviewID);
	
	// Delegate
	/*
	UFUNCTION(BlueprintCallable, Category = "Zipview Data")
	static void OnResultSet(const FOnResult& callback);
	
	UPROPERTY(BlueprintAssignable, Category = "Zipview Data")
	FOnResult OnResult;
	*/
	
private:
	// Delegate

};


