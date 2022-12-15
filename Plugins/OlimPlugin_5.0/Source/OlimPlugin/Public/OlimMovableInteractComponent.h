// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OlimInteractInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "OlimMovableInteractComponent.generated.h"

// Property 구조체
USTRUCT(Atomic, BlueprintType)
struct FOlimMovableProperty
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
enum class EOlimMovableComponentState : uint8
{
	EOMCS_Wait UMETA(DisplayName = "Wait"),
	EOMCS_Moving UMETA(DisplayName = "Moving"),

	EOMCS_DefaultMAX UMETA(DisplayName = "MAX")
};

// 현재 상태 Enum (Open/Close)
UENUM(BlueprintType)
enum class EOlimMovementState : uint8
{
	EOMS_Open UMETA(DisplayName = "Open"),
	EOMS_Close UMETA(DisplayName = "Close"),

	EOMS_DefaultMAX UMETA(DisplayName = "MAX")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OLIMPLUGIN_API UOlimMovableInteractComponent : public UActorComponent, public IOlimInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOlimMovableInteractComponent();

	// BeginPlay
	virtual void BeginPlay() override;

	// Tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Interact List 에서 property 얻어오기
	void GetProperty(const FString& name);

	// 상호작용
	void Interaction(TOptional<FOlimMovableProperty>& property, TObjectPtr<UStaticMeshComponent> mesh);

	// Interface 함수
	virtual void Interact(FString string,EOlimActorType type) override;
	
	// Interface 함수 - BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractBP(const FString& str,EOlimActorType type);
	virtual void InteractBP_Implementation(const FString& str, EOlimActorType type) override;
	
public:
	// 현재 상태
	UPROPERTY(BlueprintReadOnly)
	EOlimMovableComponentState ComponentState = EOlimMovableComponentState::EOMCS_Wait;

	UPROPERTY(BlueprintReadWrite)
	EOlimMovementState MovementState = EOlimMovementState::EOMS_Close;

	// Interact List
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Product")
	TMap<FString, FOlimMovableProperty> MovableList;

	// 현재 Property
	TOptional<FOlimMovableProperty> CurrentProperty;
	
	// Interact Actor
	UPROPERTY()
	TObjectPtr<class AOlimInteractActor> InteractActor;
#pragma region Timeline
	// 타임라인
	FTimeline InteractTimeline;

	// 타임라인 이벤트 - Curve 사용
	UFUNCTION(BlueprintCallable)
	void PlayTimeline(UCurveFloat* curve);

	// 타임라인 시간 얻어오기
	UFUNCTION()
	void TimelineProgress(float value);

	// 타임라인 종료 이벤트
	UFUNCTION()
	void TimelineFinish();

	// 타임라인 시간 - Lerp 에서 사용하는 Alpha
	UPROPERTY(BlueprintReadOnly)
	float TimelineAlpha;
#pragma endregion Timeline
};
