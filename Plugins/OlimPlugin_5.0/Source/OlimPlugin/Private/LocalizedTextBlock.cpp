// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalizedTextBlock.h"
#include "../Public/ClientConfigFunctionLibrary.h"
#include "../Public/DefineStructure.h"

#define LOCTEXT_NAMESPACE "UMG"

ULocalizedTextBlock::ULocalizedTextBlock()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/OlimPlugin/etc/CommonLocalizeTable"));
	if (DataTable.Succeeded())
	{
		LocalizingDataTable = DataTable.Object;
	}
}

void ULocalizedTextBlock::SynchronizeProperties()
{
	UTextBlock::SynchronizeProperties();

	Transfer(KeyString, NoTransfer);
}

#if WITH_EDITOR

void ULocalizedTextBlock::OnCreationFromPalette()
{
	UTextBlock::OnCreationFromPalette();
}

const FText ULocalizedTextBlock::GetPaletteCategory()
{
	return LOCTEXT("", "OlimPlanet Custom Stuff");
}

#endif

bool ULocalizedTextBlock::Transfer(const FString& Key, bool dontTransfer)
{
	NoTransfer = dontTransfer;
	if (!NoTransfer)
	{
		KeyString = Key;
		if (LocalizingDataTable == nullptr)
			return false;

		Refresh();

	}
	return false;
}

bool ULocalizedTextBlock::Refresh()
{
	if (KeyString.IsEmpty() == false)
	{
		bool b; int i; float f; FString s;
		UClientConfigFunctionLibrary::GetClientConfig("Language", b, i, f, s);

		if (FLocalizingTableRow* r = LocalizingDataTable->FindRow<FLocalizingTableRow>(FName(*KeyString), FString("")))
		{
			if (s == "Ko")
			{
				SetText(FText::FromString(r->Ko));
			}
			else
			{
				SetText(FText::FromString(r->En));
			}
			return true;
		}

		else
		{
			SetText(FText::FromString(KeyString));
			return false;
		}
	}

	else
	{
		return false;
	}
}

//
#undef LOCTEXT_NAMESPACE