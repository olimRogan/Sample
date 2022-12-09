// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "Components/Button.h"

void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button.Get()->OnClicked.AddDynamic(this,&UItemWidget::OnClick);
}
