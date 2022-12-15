// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Styling/SlateBrush.h"
#include "Engine/TargetPoint.h"
#include "Engine/StaticMesh.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "ObjectInfoIndicatorBase.generated.h"

UENUM(BlueprintType)
enum class ObjectInfoType : uint8
{
	NONE			UMETA(DisplayName = "None"),
	IMAGE			UMETA(DisplayName = "Image"),
	VIDEO_URL		UMETA(DisplayName = "VideoUrl"),
	WEB_PAGE		UMETA(DisplayName = "WebPage480"),
	MODEL_3D		UMETA(DisplayName = "Model3D"),
	OS_BROWSER		UMETA(DisplayName = "OS Browser"),
	CHATTING		UMETA(DisplayName = "Chatting"),
};

USTRUCT(Atomic, BlueprintType)
struct FMeshData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
		FVector ScaleOffset = FVector(1.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
		FVector PositionOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
		FString TargetURL;
};

UCLASS()
class OLIMPLUGIN_API AObjectInfoIndicatorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectInfoIndicatorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	float FloorHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	FSlateBrush IconUnhoverImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	FSlateBrush IconHoverImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	TArray<ATargetPoint*> PointList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	ObjectInfoType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	TArray<FSlateBrush> ImageList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	TArray<FString> MediaUrlList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	TArray<FString> WebPageUrlList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	TArray<FMeshData> StaticMeshList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	FString TextLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	FString IconID;
	
public:
	const bool IsValidTargetPoint(const ATargetPoint* pTP);
	TSharedPtr<FJsonValue> DumpWithPoint(const ATargetPoint* pTP);
};
