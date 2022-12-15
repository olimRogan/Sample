// Fill out your copyright notice in the Description page of Project Settings.

#include "IsoCaptureHelper.h"
#if WITH_EDITOR
#include "Editor.h"
#include "EditorViewportClient.h"
#endif

AIsoCaptureHelper::AIsoCaptureHelper()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	ScreenShotSize = FIntPoint(640, 320);
}

void AIsoCaptureHelper::Capture()
{
#if WITH_EDITOR
	TArray<FEditorViewportClient*> Viewports = GEditor->GetAllViewportClients();
	for (FEditorViewportClient* Client : Viewports)
	{
		if (Client->IsVisible() == true)
		{
			TargetClient = Client;
			break;
		}
	}
	if (TargetClient == nullptr)
		return;
	AngleIndex = 0;
	Interval = 1.f;
	bInProcess = true;
	TargetClient->SetRealtime(true);
	TargetClient->EngineShowFlags.SetGrid(false);
	TargetClient->SetPreviewScreenPercentage(200);
#endif
}

void AIsoCaptureHelper::Stop()
{
#if WITH_EDITOR
	bInProcess = false;
	TargetClient->SetRealtime(false);
	TargetClient->EngineShowFlags.SetGrid(true);
	UE_LOG(LogTemp, Log, TEXT("ISO Capture Stopped!!"), AngleIndex + 1, CaptureCount);
#endif
}

bool AIsoCaptureHelper::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AIsoCaptureHelper::Tick(float DeltaSeconds)
{
#if WITH_EDITOR
	if (bInProcess == false)
		return;
	if (Interval > 0.f && AngleIndex < CaptureCount)
	{
		Interval -= DeltaSeconds;
		return;
	}

	TArray<FEditorViewportClient*> Viewports = GEditor->GetAllViewportClients();
	for (FEditorViewportClient* Client : Viewports)
	{
		if (Client->IsVisible() == true)
		{
			TargetClient = Client;
			break;
		}
	}
	if (TargetClient == nullptr)
		return;
	TargetClient->bDrawAxes = false;
	if (TargetClient->IsSetShowGridChecked())
		TargetClient->SetShowGrid();

	float Angle = AngleIndex * (360.f / CaptureCount);
	FVector TargetCameraPosition;
	TargetCameraPosition.X = TargetPoint.X + Length * FMath::Cos(FMath::DegreesToRadians<float>(Angle)) * FMath::Cos(FMath::DegreesToRadians<float>(Theta));
	TargetCameraPosition.Y = TargetPoint.Y + Length * FMath::Sin(FMath::DegreesToRadians<float>(Angle)) * FMath::Cos(FMath::DegreesToRadians<float>(Theta));
	TargetCameraPosition.Z = TargetPoint.Z + Length * FMath::Sin(FMath::DegreesToRadians<float>(Theta));
	UE_LOG(LogTemp, Log, TEXT("index: %d / %d"), AngleIndex + 1, CaptureCount);
	auto d = TargetPoint - TargetCameraPosition;
	TargetClient->SetInitialViewTransform(ELevelViewportType::LVT_Perspective, TargetCameraPosition, d.Rotation(), false);
	FString f = TEXT("HighResShot ");
	f.Append(FString::FromInt(ScreenShotSize.X)).Append(TEXT("x")).Append(FString::FromInt(ScreenShotSize.Y));
	GEngine->Exec(nullptr, *f);

	Interval = 1.f;
	AngleIndex++;
	if (AngleIndex >= CaptureCount)
	{
		bInProcess = false;
		TargetClient->SetRealtime(false);
	}
#endif
}
