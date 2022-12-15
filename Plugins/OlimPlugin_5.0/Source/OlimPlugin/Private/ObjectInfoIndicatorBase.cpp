// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectInfoIndicatorBase.h"

// Sets default values
AObjectInfoIndicatorBase::AObjectInfoIndicatorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectInfoIndicatorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectInfoIndicatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const bool AObjectInfoIndicatorBase::IsValidTargetPoint(const ATargetPoint * pTP)
{
	for (auto pPL : PointList)
	{
		if (pTP == pPL)
			return true;
	}
	return false;
}

TSharedPtr<FJsonValue> AObjectInfoIndicatorBase::DumpWithPoint(const ATargetPoint * pTP)
{
	if (IsValidTargetPoint(pTP) == false)
		return nullptr;

	TSharedPtr<FJsonObject> jResult = MakeShareable(new FJsonObject);
	jResult->SetStringField("title", TextLabel);

	if (Type == ObjectInfoType::NONE)
		jResult->SetStringField("hotspotType", "");
	else if (Type == ObjectInfoType::IMAGE)
		jResult->SetStringField("hotspotType", "image");
	else if (Type == ObjectInfoType::VIDEO_URL)
		jResult->SetStringField("hotspotType", "video");
	else if (Type == ObjectInfoType::WEB_PAGE)
		jResult->SetStringField("hotspotType", "iframe");
	else if (Type == ObjectInfoType::OS_BROWSER)
		jResult->SetStringField("hotspotType", "external");
	else if (Type == ObjectInfoType::MODEL_3D)
		jResult->SetStringField("hotspotType", "model");
	else
		jResult->SetStringField("hotspotType", "");

	jResult->SetStringField("action", "");

	auto Direction = GetActorLocation() - pTP->GetActorLocation();
	Direction.Normalize();
	auto Yaw = Direction.Rotation().Yaw - pTP->GetActorForwardVector().Rotation().Yaw + 180.f;
	auto Pitch = Direction.Rotation().Pitch - pTP->GetActorForwardVector().Rotation().Pitch;
	jResult->SetNumberField("yaw", FMath::DegreesToRadians<float>(Yaw));
	jResult->SetNumberField("pitch", -FMath::DegreesToRadians<float>(Pitch));	
	
	return MakeShareable(new FJsonValueObject(jResult));
}
