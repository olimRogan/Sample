// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViewPointManager.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FViewPoint
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	AActor* ViewPointActor;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName ViewPointName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<FName> ThemeEventNames;

};

UCLASS()
class UE5CPPSAMPLE_API AViewPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViewPointManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TArray<FViewPoint>& GetViewPoints() const;
	int GetViewPointsNum()const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:



public:

protected:

private:
	//need SetUp Data in OutLiner details.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Container")
		TArray<FViewPoint> ViewPoints;
};
