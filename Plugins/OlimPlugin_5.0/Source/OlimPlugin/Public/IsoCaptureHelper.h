// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IsoCaptureHelper.generated.h"

UCLASS()
class OLIMPLUGIN_API AIsoCaptureHelper : public AActor
{
	GENERATED_BODY()
public:
	AIsoCaptureHelper();

public:
	UPROPERTY(Category = "Capture", EditAnywhere)
		FIntPoint ScreenShotSize;
	UPROPERTY(Category = "Capture", EditAnywhere)
		FVector TargetPoint;
	UPROPERTY(Category = "Capture", EditAnywhere)
		float Length = 1000.f;
	UPROPERTY(Category = "Capture", EditAnywhere)
		float Theta = 45.f;
	UPROPERTY(Category = "Capture", EditAnywhere)
		int CaptureCount = 36;
	UFUNCTION(Category = "Capture", meta = (CallInEditor = "true"))
		void Capture();
	UFUNCTION(Category = "Capture", meta = (CallInEditor = "true"))
		void Stop();

public:
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void Tick(float DeltaSeconds) override;
private:
	class FEditorViewportClient* TargetClient = nullptr;


private:
	bool bInProcess = false;
	float Interval = 0.f;
	int AngleIndex = 0;
};
