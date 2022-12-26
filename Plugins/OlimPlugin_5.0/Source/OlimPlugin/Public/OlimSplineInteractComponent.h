// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OlimInteractInterface.h"
#include "Components/ActorComponent.h"
#include "OlimSplineInteractComponent.generated.h"

// 현재 상태 Enum
UENUM(BlueprintType)
enum class EOlimSplineComponentState : uint8
{
	EOSCS_Off UMETA(DisplayName = "Off"),
	EOSCS_On UMETA(DisplayName = "On"),

	EOSCS_DefaultMAX UMETA(DisplayName = "MAX")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),HideCategories = ("Activation","Cooking","Collision","AssetUserData"))
class OLIMPLUGIN_API UOlimSplineInteractComponent : public UActorComponent, public IOlimInteractInterface
{
	GENERATED_BODY()

public:	
	UOlimSplineInteractComponent();

protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interface 함수
	virtual void Interact(const FString& string,EOlimActorType type) override;
	
	// Interface 함수 - BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void InteractBP(const FString& str,const EOlimActorType type);
	virtual void InteractBP_Implementation(const FString& str,const EOlimActorType type) override;

	// Interact Actor
	UPROPERTY()
	TObjectPtr<class AOlimInteractActor> OlimInteractActor;

	// Spline Actor
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category = "Product|Spline")
	TObjectPtr<class AOlimSplineActor> SplineActor;
	
	// 이동 속도
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Product|Spline")
	float Speed;

	// 회전 속도
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Product|Spline")
	float TurnSpeed;

	UPROPERTY()
	EOlimSplineComponentState State;
	
	// 처음 회전값
	UPROPERTY()
	FRotator InitRotation;

	// Spline Time
	UPROPERTY() 
	float Time;

	// 현재 Spline 길이
	UPROPERTY()
	float CurrentLength = 0;
};
