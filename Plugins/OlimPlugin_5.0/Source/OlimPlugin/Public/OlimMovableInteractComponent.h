// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OlimInteractInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "OlimMovableInteractComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FOlimTransform
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property" ,meta = (ClampMin=-360,ClampMax=360))
	FRotator Rotation;
};

// Property 구조체
USTRUCT(Atomic, BlueprintType)
struct FOlimMovableProperty
{
	GENERATED_BODY()
public:
	// 시작 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property")
	FOlimTransform FromTransform;
	// 끝 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property")
	FOlimTransform ToTransform;
	// Float Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property")
	TObjectPtr<UCurveFloat> Curve;
	// 역방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Property")
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
	EOMS_Close UMETA(DisplayName = "Close"),
	EOMS_Open UMETA(DisplayName = "Open"),
	
	EOMS_DefaultMAX UMETA(DisplayName = "MAX")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),HideCategories = ("Activation","Cooking","Collision","AssetUserData"))
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
	void Interaction(const TOptional<FOlimMovableProperty>& property, TObjectPtr<UStaticMeshComponent> mesh);

	// Interface 함수
	virtual void Interact(const FString& string,EOlimActorType type) override;
	
	// Interface 함수 - BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void InteractBP(const FString& str,const EOlimActorType type);
	virtual void InteractBP_Implementation(const FString& str,const EOlimActorType type) override;
	
public:
	// 컴포넌트 동작 상태
	UPROPERTY(BlueprintReadOnly, Category = "Enum")
	EOlimMovableComponentState ComponentState = EOlimMovableComponentState::EOMCS_Wait;

	// 동작 상태 (Open/Close)
	UPROPERTY(BlueprintReadWrite, Category = "Enum")
	EOlimMovementState MovementState = EOlimMovementState::EOMS_Close;

	// Interact List
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Product")
	TMap<FString, FOlimMovableProperty> MovableList = {{"Open",FOlimMovableProperty()},{"Close",FOlimMovableProperty()}};

	// 현재 Property
	TOptional<FOlimMovableProperty> CurrentProperty;
	
	// Interact Actor
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent>  MeshComponent;
#pragma region Timeline
	// 타임라인
	FTimeline InteractTimeline;

	// 타임라인 이벤트 - Curve 사용
	UFUNCTION()
	void PlayTimeline(UCurveFloat* curve);

	// 타임라인 시간 얻어오기
	UFUNCTION()
	void TimelineProgress(float value);

	// 타임라인 종료 이벤트
	UFUNCTION()
	void TimelineFinish();

	// 타임라인 시간 - Lerp 에서 사용하는 Alpha
	UPROPERTY(BlueprintReadOnly, Category = "Timeline")
	float TimelineAlpha;
#pragma endregion Timeline
};
