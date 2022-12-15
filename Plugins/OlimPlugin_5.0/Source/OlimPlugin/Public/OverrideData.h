// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverrideData.generated.h"

USTRUCT(BlueprintType, Atomic)
struct OLIMPLUGIN_API FOverrideDataStructure
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
		UTexture2D* Logo;

	FOverrideDataStructure& operator=(const FOverrideDataStructure& r)
	{
		Logo = r.Logo;
		return *this;
	}
};

UCLASS()
class OLIMPLUGIN_API AOverrideData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverrideData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Etc")
	FOverrideDataStructure Data;
};
