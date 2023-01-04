// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OlimInteractInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "OlimLightInteractComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FOlimLightProperty
{
	GENERATED_BODY()
public:
	// 처음 밝기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float FromIntensity;
	// 끝 밝기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float ToIntensity;
	// 처음 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	FLinearColor FromColor;
	// 끝 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	FLinearColor ToColor;
	// Float Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	TObjectPtr<UCurveFloat> Curve;
	// 역방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	bool bReverse;
};

// 현재 상태 Enum
UENUM(BlueprintType)
enum class EOlimLightComponentState : uint8
{
	EOLCS_Off UMETA(DisplayName = "Off"),
	EOLCS_On UMETA(DisplayName = "On"),

	EOLCS_DefaultMAX UMETA(DisplayName = "MAX")
};

// 현재 상태 Enum
UENUM(BlueprintType)
enum class EOlimLightState : uint8
{
	EOLS_Off UMETA(DisplayName = "Off"),
	EOLS_On UMETA(DisplayName = "On"),

	EOLS_DefaultMAX UMETA(DisplayName = "MAX")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),HideCategories = ("Activation","Cooking","Collision","AssetUserData"))
class OLIMPLUGIN_API UOlimLightInteractComponent : public UActorComponent, public IOlimInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOlimLightInteractComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// List 에서 property 얻어오기
	void GetProperty(const FString& name);
	
	// 상호작용
	void Interaction(const TOptional<FOlimLightProperty>& property,class ULightComponent* lightComp);

	// Interface 함수
	virtual void Interact(const FString& string,EOlimActorType type) override;
	
	// Interface 함수 - BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void InteractBP(const FString& str,const EOlimActorType type);
	virtual void InteractBP_Implementation(const FString& str,const EOlimActorType type) override;

public:
	// 컴포넌트가 동작 상태
	UPROPERTY(BlueprintReadOnly, Category = "Enum")
	EOlimLightComponentState ComponentState = EOlimLightComponentState::EOLCS_Off;

	// Light 상태 (On/Off)
	UPROPERTY(BlueprintReadWrite, Category = "Enum")
	EOlimLightState LightState = EOlimLightState::EOLS_Off;

	// Interact List
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Product")
	TMap<FString, FOlimLightProperty> LightList = {{"On",FOlimLightProperty()},{"Off",FOlimLightProperty()}};

	UPROPERTY()
	TObjectPtr<class AOlimLightActor> LightActor;

	// 현재 Property
	TOptional<FOlimLightProperty> CurrentProperty;

	// Interact Actor
	UPROPERTY()
	TObjectPtr<class AOlimInteractActor> InteractActor;

	UPROPERTY()
	TObjectPtr<ULightComponent> LightComponent;

#pragma region Timeline
	// 타임라인
	FTimeline InteractTimeline;

	// 타임라인 이벤트 - Curve 사용
	UFUNCTION()
	void PlayTimeline(UCurveFloat* curve);

	// 타임라인 시간 얻어오기
	UFUNCTION()
	void TimelineProgress(float Value);

	// 타임라인 종료 이벤트
	UFUNCTION()
	void TimelineFinish();

	// 타임라인 시간 - Lerp 에서 사용하는 Alpha
	UPROPERTY(BlueprintReadOnly, Category = "Timeline")
	float TimelineAlpha;
#pragma endregion Timeline	
};
