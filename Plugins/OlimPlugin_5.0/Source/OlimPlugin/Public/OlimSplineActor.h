// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OlimSplineActor.generated.h"

UCLASS()
class OLIMPLUGIN_API AOlimSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOlimSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// BillBoard Component
	UPROPERTY(EditAnywhere,meta= (AllowPrivateAccess = "true"), Category = "Component")
	class UBillboardComponent* BillboardComponent;

	// Spline Component
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Product|Spline", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class USplineComponent> SplineComponent;

public:
	// Spline Component Getter
	UFUNCTION(BlueprintCallable, Category = "Function")
	USplineComponent* GetSpline() const {return SplineComponent;}
};
