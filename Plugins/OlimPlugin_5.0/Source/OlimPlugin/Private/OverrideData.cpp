// Fill out your copyright notice in the Description page of Project Settings.


#include "OverrideData.h"

// Sets default values
AOverrideData::AOverrideData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOverrideData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOverrideData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

