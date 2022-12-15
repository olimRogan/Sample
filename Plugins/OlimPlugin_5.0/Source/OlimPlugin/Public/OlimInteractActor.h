// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OlimInteractActor.generated.h"

UCLASS()
class OLIMPLUGIN_API AOlimInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOlimInteractActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Root Scene
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	TObjectPtr<USceneComponent> RootScene;

	// Billboard 
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	TObjectPtr<UBillboardComponent> BillboardComponent;

	// Mesh Component
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Product")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
private:
	// Interact Component - Movable
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimMovableInteractComponent> MovableInteractComponent;
	
	// Interact Component - Light
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimLightInteractComponent> LightInteractComponent;
	

	// 마우스 클릭 반응 체크
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Product", meta =(AllowPrivateAccess = "true"))
	bool bEnableClick;

};
