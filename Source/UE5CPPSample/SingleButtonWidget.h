// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SingleButtonWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectDelegate, FName, Name);
UCLASS()
class UE5CPPSAMPLE_API USingleButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
protected:
	virtual void NativeConstruct() override;

	
private:

public:

	UPROPERTY(EditAnywhere, BlueprintAssignable,BlueprintCallable, Category = "Delegate")
		FSelectDelegate SelectThemeDelegate;

	UPROPERTY(BlueprintReadWrite)
		FName BindedText;
protected:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
		class UTextBlock* DisplayName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Button;
private:


};
