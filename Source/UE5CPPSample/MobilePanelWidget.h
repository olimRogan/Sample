// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SingleButtonWidget.h"
#include "MobilePanelWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDisplayType : uint8
{
	Home = 0,
	ViewpointList,
	ThemeList,
};


UCLASS()
class UE5CPPSAMPLE_API UMobilePanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Init();
	virtual void Init_Implementation();
//old
	/*UFUNCTION(BlueprintCallable)
		USingleButtonWidget* CreateThemeWidget(FName themeName = "NONE");
	UFUNCTION(BlueprintCallable)
		void CreateThemeWidgets(int32 count);*/

	UFUNCTION(BlueprintCallable)
		void ChangeThemeData(int32 currentIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		USingleButtonWidget* CreateThemeWidget(FName themeName = "NONE");

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CreateThemeWidgets(int32 count); 

	UFUNCTION(BlueprintCallable)
		void VisibleAllTheme(ESlateVisibility eVisibility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void AddChildByData();

	UFUNCTION(BlueprintCallable)
		void ExecuteThemeEvent(FName themeEventName);

private:

public:
	const int32 InitThemeCounts = 10;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDisplayType ECurrentDisplayType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		FName CurrentLocationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		int32 CurrentLocationIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
		TObjectPtr<class AThemeEventManager> ThemeEventManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
		TObjectPtr<class AViewPointManager> ViewPointManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
		TArray<USingleButtonWidget*> ThemeWidgets;
private:
	
};
