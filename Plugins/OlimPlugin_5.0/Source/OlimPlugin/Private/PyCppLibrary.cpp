// Fill out your copyright notice in the Description page of Project Settings.

#include "PyCppLibrary.h"

#if WITH_EDITOR
#include "EditorLevelLibrary.h"
#include "ObjectInfoIndicatorBase.h"

void UPyCppLibrary::InsertTargetPoint()
{
	auto aa = UEditorLevelLibrary::GetSelectedLevelActors();
	AObjectInfoIndicatorBase* indicator = nullptr;
	TArray<ATargetPoint*> target_point_list;
	for (auto a : aa)
	{
		if (auto p = Cast<ATargetPoint>(a))
		{
			target_point_list.Add(p);
		}
		if (auto p = Cast<AObjectInfoIndicatorBase>(a))
		{
			indicator = p;
		}
	}

	if (indicator != nullptr)
	{
		indicator->PointList = target_point_list;
		UE_LOG(LogTemp, Log, TEXT("indicator : %s"), *indicator->GetName());
		for (auto p : indicator->PointList)
		{
			UE_LOG(LogTemp, Log, TEXT("point : %s"), *p->GetName());
		}
	}
}

#endif