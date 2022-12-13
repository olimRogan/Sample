// Fill out your copyright notice in the Description page of Project Settings.


#include "MobilePanelWidget.h"
#include "EngineUtils.h"
#include "InteractInterface.h"

#include "ThemeEventManager.h"
#include "ViewPointManager.h"
#include "Blueprint/WidgetTree.h"

void UMobilePanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UMobilePanelWidget::Init()
{
	UWorld* world = GetWorld();	

	for (const auto& themeEventManager : TActorRange<AThemeEventManager>(world))
	{
		ThemeEventManager = themeEventManager;

		if ( IsValid(ThemeEventManager))
		{
			break; 
		}
	}

	for (const auto& viewPointManager : TActorRange<AViewPointManager>(world))
	{
		ViewPointManager = viewPointManager;

		if (IsValid(ViewPointManager))
		{
			break;
		}
	}

	CreateThemeWidgets(InitThemeCounts);
	ChangeThemeData(CurrentLocationIndex);
}

USingleButtonWidget* UMobilePanelWidget::CreateThemeWidget(FName themeName)
{
	if(ChildClass.Get())
	{
		USingleButtonWidget* newWidget = CreateWidget<USingleButtonWidget>(this, ChildClass.Get());
		newWidget->BindedText = themeName;
		return newWidget;
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,TEXT("Child Class nullptr"));
	return nullptr;
}

void UMobilePanelWidget::ChangeThemeData(int32 currentIndex)
{

	//int viewPointCounts = ViewPointManager->GetViewPoints().Num();
	int viewPointCounts = ViewPointManager->GetViewPointsNum();
	int nextIndex = currentIndex + 1;

	if (nextIndex >= viewPointCounts)
	{
		nextIndex = 0;
	}

	FViewPoint viewPoint = ViewPointManager->GetViewPoints()[nextIndex];

	//if need ThemeWidget, create..
	int32 ThemeWidgetCount = ThemeWidgets.Num();
	int32 ThemeCount = viewPoint.ThemeEventNames.Num();

	if (ThemeWidgetCount < ThemeCount)
	{
		int32 needCount = viewPointCounts - ThemeCount;
		CreateThemeWidgets(needCount);
		AddChildByData();
	}

	// change data
	USingleButtonWidget* themeWidget = nullptr;
	FName themeName = {};
	for (int32 index = 0; index < ThemeCount; index++ )
	{
		if(ThemeWidgets.IsEmpty()) break;
		
		themeWidget = ThemeWidgets[index];

		if ( !IsValid(themeWidget))
		{
			break;
		}

		themeName = viewPoint.ThemeEventNames[index];

		themeWidget->SetVisibility(ESlateVisibility::Visible);
		themeWidget->BindedText = themeName;
		
	}

	CurrentLocationIndex = nextIndex;
	CurrentLocationName = viewPoint.ViewPointName;
}

void UMobilePanelWidget::CreateThemeWidgets(int32 count)
{
	USingleButtonWidget* newWidget = nullptr;

	for( int32 index = 0; index < count; index++)
	{
		newWidget = CreateThemeWidget();

		if (IsValid(newWidget))
		{
			ThemeWidgets.Add((newWidget));
			newWidget->SelectThemeDelegate.AddDynamic(this,&UMobilePanelWidget::ExecuteThemeEvent);
		}
				
	}

	VisibleAllTheme(ESlateVisibility::Hidden);
}

void UMobilePanelWidget::VisibleAllTheme(ESlateVisibility eVisibility)
{
	for ( auto& theme : ThemeWidgets)
	{
		theme->SetVisibility(eVisibility);
	}
}

void UMobilePanelWidget::ExecuteThemeEvent(FName themeEventName)
{
	const FThemeEvent* themeEvent =  ThemeEventManager->GetThemeEvents()->Find(themeEventName);

	if (themeEvent)
	{		
		for( auto& dataThemeEvent : themeEvent->DataThemeEvents)
		{
			
			AActor* actor = dataThemeEvent.InteractableMarker;
			if ( IsValid(actor))
			{
				if (actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
				{
					IInteractInterface::Execute_InteractBP(actor, dataThemeEvent.ExecutableEventName.ToString(), EActorType::EAS_Marker);
				}
			}
		}
	}
	
}
