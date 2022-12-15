// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimDataCommunication.h"
#include "OlimBlueprintFunctionLibrary.h"
#include "OlimGameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Containers/UnrealString.h"


void UOlimDataCommunication::GetProductAllInfoData(const FString URL)
{
	auto Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindStatic(&UOlimDataCommunication::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb("Get");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->ProcessRequest();
}


void UOlimDataCommunication::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	Request->OnProcessRequestComplete().Unbind();

	FString Result;
	FString ZipviewID;

	//int ResponseCode;
	//ResponseCode = Response->GetResponseCode();
	//if (ResponseCode == 200)
	if(Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		Result = Response->GetContentAsString();
		
		TSharedPtr<FJsonObject> JsonToLoad;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Result);
		
		if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad) && JsonToLoad.IsValid())
		{
			if (JsonToLoad->HasField("Code")) 
			{
				ZipviewID = JsonToLoad->GetStringField("Code");
				UOlimBlueprintFunctionLibrary::SaveStringToFile(Result, "Unique/" + ZipviewID + "/Development/03_ETC", "ProductInfo.json");
				//OnResult.Broadcast(EDownloadResult::Success);
			}
		}
		else
		{
			if(Result.Len() > 10)
			{
				Result = "{ \"GuideBook\" : " + Result + "}";
				TSharedRef<TJsonReader<TCHAR>> insJsonReader = TJsonReaderFactory<TCHAR>::Create(Result);

				if (FJsonSerializer::Deserialize(insJsonReader, JsonToLoad) && JsonToLoad.IsValid())
				{
					TArray<TSharedPtr<FJsonValue>> insInfoArray = JsonToLoad->GetArrayField("GuideBook");
					TSharedPtr<FJsonObject> insJson = insInfoArray[0]->AsObject();
					if(insJson->HasField("page"))
					{
						ZipviewID = insJson->GetStringField("page");
						UOlimBlueprintFunctionLibrary::SaveStringToFile(Result, "Unique/" + ZipviewID + "/Development/03_ETC", "GuideBookInfo.json");
					}
				}
			}
		}
	}
	else
	{

	}
}


FGuideBookInfo UOlimDataCommunication::GuideBookFilter(FString ZipviewID)
{
	FGuideBookInfo Result;

	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipviewID + "/Development/03_ETC", "GuideBookInfo.json");
	TSharedPtr<FJsonObject> JsonToLoad = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	Result.isResultVailed = false;

	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad) && JsonToLoad.IsValid())
	{
		Result.isResultVailed = true;
		
		Result.Subject.Empty();
		Result.WebPath.Empty();
		Result.Format.Empty();

		TArray<TSharedPtr<FJsonValue>> InfoArray = JsonToLoad->GetArrayField("GuideBook");
		
		for (int32 i = 0; i < InfoArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> InfoJson;
			InfoJson = InfoArray[i]->AsObject();
			if (InfoJson->HasField("metadata"))
			{
				TArray<TSharedPtr<FJsonValue>> insInfoArray;
				insInfoArray = ((InfoJson->GetObjectField("metadata"))->GetObjectField("configuration"))->GetArrayField("properties");

				if((insInfoArray[0]->AsObject())->GetStringField("value") == "pdf")
				{
					Result.Format.Add("pdf");
					Result.Subject.Add(InfoJson->GetStringField("subject"));

					TArray<TSharedPtr<FJsonValue>> insInfoArray2 = InfoJson->GetArrayField("media");
					Result.WebPath.Add((insInfoArray2[0]->AsObject())->GetStringField("path"));
				}
			}
		}
	}
	return Result;
}


FProductionBasicInfo UOlimDataCommunication::ProductInfoFilter(FString ZipviewID)
{
	FProductionBasicInfo Result;
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipviewID + "/Development/03_ETC", "ProductInfo.json");
	TSharedPtr<FJsonObject> JsonToLoad = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	Result.isResultVailed = false;

	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad) && JsonToLoad.IsValid())
	{
		Result.isResultVailed = true;
		Result.SourceType.Empty();

		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Name"));
		Result.BasicInfoMap.Add("Name", JsonToLoad->GetStringField("Name"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Address"));
		Result.BasicInfoMap.Add("Address", JsonToLoad->GetStringField("Address"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Developer"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Constructor"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("ConstructDate"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Category"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("Plottage"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("GrossArea"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("BuildingArea"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("BuildingLandRatio"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("FloorAreaRatio"));
		
		FString insHouseholdDes = JsonToLoad->GetStringField("HouseholdDescription");
		if (insHouseholdDes.Len() > 0) insHouseholdDes = "#" + JsonToLoad->GetStringField("HouseholdDescription");
		else insHouseholdDes = "";

		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("TotalHousehold") + insHouseholdDes);
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("MinMaxFloor"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("TotalParking"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("ParkingPerHousehold"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("HeatingMethod"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("MainNumber"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("HouseholdRatio"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("NameEN"));
		Result.BasicInfoMap.Add("NameEN", JsonToLoad->GetStringField("NameEN"));
		Result.BasicInfoAll.Add(JsonToLoad->GetStringField("AddressEN"));
		Result.BasicInfoMap.Add("AddressEN", JsonToLoad->GetStringField("AddressEN"));

		Result.SourceType.Empty();
		Result.ImagesName.Empty();
		Result.ImagesURL.Empty();

		TArray<TSharedPtr<FJsonValue>> insInfoArray = JsonToLoad->GetArrayField("Images");
		TSharedPtr<FJsonObject> insImagesJson;

		for (int32 i = 0; i < insInfoArray.Num(); i++)
		{		
			insImagesJson = insInfoArray[i]->AsObject();
			Result.SourceType.Add(insImagesJson->GetStringField("SourceType"));
			TSharedPtr<FJsonObject> insImagesJson2 = insImagesJson->GetObjectField("Source");
			Result.ImagesName.Add(insImagesJson2->GetStringField("FileName"));
			Result.ImagesURL.Add(insImagesJson2->GetStringField("Url"));
		}
	}
	return Result;
}


FProductionTypeInfo UOlimDataCommunication::TypeInfoFilter(FString ZipviewID, FString TypeCode)
{
	FProductionTypeInfo Result;
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipviewID + "/Development/03_ETC", "ProductInfo.json");
	TSharedPtr<FJsonObject> JsonToLoad = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	Result.isResultVailed = false;

	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad) && JsonToLoad.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> insInfoArray = JsonToLoad->GetArrayField("Types");
		TSharedPtr<FJsonObject> insTypeJson;
		
		for (int i = 0; i < insInfoArray.Num(); i++)
		{
			insTypeJson = insInfoArray[i]->AsObject();
			if (insTypeJson->GetStringField("TypeCode") == TypeCode)
			{
				Result.TypeInfoAll.Empty();

				Result.TypeInfoAll.Add(insTypeJson->GetStringField("Name") + " - " + insTypeJson->GetStringField("Type"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("SupplyArea"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("UseArea"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("Households"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("Rooms"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("Restrooms"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("AnnualAverageCost"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("SummerAverageCost"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("WinterAverageCost"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("TradeRTP"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("LeaseRTP"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("RentRTP"));
				Result.TypeInfoAll.Add(insTypeJson->GetStringField("NameEN") + " - " + insTypeJson->GetStringField("TypeEN"));

				Result.SourceType.Empty();
				Result.ImagesURL.Empty();
				Result.ImagesName.Empty();	

				TArray<TSharedPtr<FJsonValue>> arrImages = insTypeJson->GetArrayField("Images");
				TSharedPtr<FJsonObject> insImagesJson;

				for (int32 index = 0; index < arrImages.Num(); index++)
				{
					insImagesJson = arrImages[index]->AsObject();
					Result.SourceType.Add(insImagesJson->GetStringField("SourceType"));
					TSharedPtr<FJsonObject> insImagesJson2 = insImagesJson->GetObjectField("Source");
					Result.ImagesName.Add(insImagesJson2->GetStringField("FileName"));
					Result.ImagesURL.Add(insImagesJson2->GetStringField("Url"));
				}
				Result.isResultVailed = true;
				break;
			}
		}
	}
	return Result;
}


bool UOlimDataCommunication::FileCheckAndGenerate(FString ZipviewID)
{
	bool result = true;
	FString JsonProductInfo = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipviewID + "/Development/03_ETC", "ProductInfo.json");
	FString GuideBookInfo = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipviewID + "/Development/03_ETC", "GuideBookInfo.json");

	if (JsonProductInfo.IsEmpty())
	{
		result = false;
		UOlimBlueprintFunctionLibrary::SaveStringToFile(JsonProductInfo, "Unique/" + ZipviewID + "/Development/03_ETC", "ProductInfo.json");
	}
	if (GuideBookInfo.IsEmpty())
	{
		UOlimBlueprintFunctionLibrary::SaveStringToFile(GuideBookInfo, "Unique/" + ZipviewID + "/Development/03_ETC", "GuideBookInfo.json");
	}
	return result;
}

