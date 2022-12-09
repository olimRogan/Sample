// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5CPPSAMPLE_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget),BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<class UButton> Button;

	UPROPERTY(meta=(BindWidget),BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<class UTextBlock> Text;

	UFUNCTION(BlueprintImplementableEvent)
	void OnClick();
};
