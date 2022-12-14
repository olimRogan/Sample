// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "LightInteractComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FLightProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FromIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ToIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FromColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ToColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReverse;
};

// 현재 상태 Enum
UENUM(BlueprintType)
enum class ELightState : uint8
{
	ELS_Off UMETA(DisplayName = "Off"),
	ELS_On UMETA(DisplayName = "On"),

	ES_DefaultMAX UMETA(DisplayName = "MAX")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5CPPSAMPLE_API ULightInteractComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

protected:
	ULightInteractComponent();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// List 에서 property 얻어오기
	void GetProperty(const FString& name);
	
	// 상호작용
	void Interaction(TOptional<FLightProperty>& property, TObjectPtr<ULightComponent> light);

	// Interface 함수
	virtual void Interact(FString string,EActorType type) override;
	
	// Interface 함수 - BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractBP(const FString& str,EActorType type);
	virtual void InteractBP_Implementation(const FString& str, EActorType type) override;

public:
	UPROPERTY(BlueprintReadOnly)
	ELightState State = ELightState::ELS_Off;

	// Interact List
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Product")
	TMap<FString, FLightProperty> LightList;

	// 현재 Property
	TOptional<FLightProperty> CurrentProperty;

	// Interact Actor
	UPROPERTY()
	TObjectPtr<class AInteractActor> InteractActor;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTurnOn;

#pragma region Timeline
	// 타임라인
	FTimeline InteractTimeline;

	// 타임라인 이벤트 - Curve 사용
	UFUNCTION(BlueprintCallable)
	void PlayTimeline(UCurveFloat* curve);

	// 타임라인 시간 얻어오기
	UFUNCTION()
	void TimelineProgress(float Value);

	// 타임라인 종료 이벤트
	UFUNCTION()
	void TimelineFinish();

	// 타임라인 시간 - Lerp 에서 사용하는 Alpha
	UPROPERTY(BlueprintReadOnly)
	float TimelineAlpha;
#pragma endregion Timeline
};
