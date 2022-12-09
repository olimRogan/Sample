// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FLightProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReverse;
};

UCLASS()
class UE5CPPSAMPLE_API AInteractActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
private:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UMovableInteractComponent> MovableComponent;
};
