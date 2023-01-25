// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OlimInteractActor.generated.h"

UCLASS(HideCategories = ("LOD","Misc","Physics","Streaming","Rendering"))
class OLIMPLUGIN_API AOlimInteractActor : public AActor
{
	GENERATED_BODY()

	
public:	
	AOlimInteractActor();

protected: 
	virtual void BeginPlay() override;


public:
	// Root Scene



	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	TObjectPtr<USceneComponent> RootScene = nullptr;

	// Billboard 
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	class UBillboardComponent* BillboardComponent = nullptr;

	// Billboard 
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	class UBillboardComponent* AttachPoint = nullptr;

	// Mesh Component
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Product")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
	
private:
	// Interact Component - Movable
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Component", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimMovableInteractComponent> MovableInteractComponent = nullptr;
	
	// Interact Component - Light
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Component", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimLightInteractComponent> LightInteractComponent = nullptr;

	// 태그가 같은 액터들을 상호작용하기 위해 사용
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	TArray<FString> ActorTags;
	
	// 마우스 클릭 반응 체크
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	bool bEnableClick;

	// Attach 체크
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	bool bEnableAttach;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true",EditCondition = "bEnableAttach" ,EditConditionHides))
	TMap<TObjectPtr<class AOlimInteractAttachActor>,FString> AttachActors;

	// 상호작용 잠금 - 다른 액터가 상호작용 중일 경우 반응하지 않도록 잠금
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	bool bInteractingLock;

	// 임시 기능 사용 (Main, Sub)
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	bool bUseTempFunction;

	// Main(이름 변경해야 함)
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true",EditCondition = "bUseTempFunction" ,EditConditionHides))
	TObjectPtr<AOlimInteractActor> Main;

	// Sub(이름 변경해야 함)
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true",EditCondition = "bUseTempFunction" ,EditConditionHides))
	TArray<TObjectPtr<AOlimInteractActor>> Sub;

public:
	
	TObjectPtr<UOlimLightInteractComponent> GetLightInteractComponent() const {return LightInteractComponent;}
	TObjectPtr<UOlimMovableInteractComponent> GetMovableInteractComponent() const {return MovableInteractComponent;}
};
