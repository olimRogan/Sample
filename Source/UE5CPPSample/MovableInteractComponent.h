// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MovableInteractComponent.generated.h"

// Property 구조체
USTRUCT(Atomic, BlueprintType)
struct FMovableProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform FromTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ToTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReverse;
};

// 현재 상태 Enum
UENUM(BlueprintType)
enum class EMovableState : uint8
{
	ES_Ready UMETA(DisplayName = "Ready"),
	ES_Interacting UMETA(DisplayName = "Interacting"),

	ES_DefaultMAX UMETA(DisplayName = "MAX")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5CPPSAMPLE_API UMovableInteractComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	UMovableInteractComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interact List 에서 property 얻어오기
	void GetProperty(const FString& name);

	// 상호작용
	void Interaction(TOptional<FMovableProperty> property, TObjectPtr<UStaticMeshComponent> mesh);

	// Interface 함수
	virtual void Interact(FString string) override;

	// Interface 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractBP(const FString& str);
	virtual void InteractBP_Implementation(const FString& str) override;
public:
	// 현재 상태
	EMovableState State = EMovableState::ES_Ready;

	// Interact List
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Product")
	TMap<FString, FMovableProperty> List;

	// 현재 Property
	FMovableProperty CurrentProperty;

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

	// Interaction 이 가능한지
	UPROPERTY(BlueprintReadOnly)
	bool bCanInteraction = true;

	// Interact Actor
	UPROPERTY()
	TObjectPtr<class AInteractActor> InteractActor;
};
