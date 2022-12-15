// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingButtonBase.generated.h"

class UTexture; 

UCLASS()
class OLIMPLUGIN_API AFloatingButtonBase : public AActor
{
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties
	AFloatingButtonBase();  

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	UTexture* TextureNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	UTexture* TextureHover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail")
	bool bIsBillboard;
};
