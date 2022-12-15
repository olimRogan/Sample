// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorInformations.h"
#include "OlimBlueprintFunctionLibrary.h"

InteriorInformations::InteriorInformations()
{
}

InteriorInformations::InteriorInformations(const FString & zipviewID)
{
	ZipViewID = zipviewID;
	SetProductionState();
	SetInteriorSelectImages();
}

InteriorInformations::~InteriorInformations()
{
}

void InteriorInformations::SetInteriorSelectImages()
{
	arrInteriorSelectImg.Empty();

	FString NormalImgName;
	FString HoverImgName;
	FString InfoImgName;

	// 이미지들을 해당 폴더에서 전부 가져온다.
	TArray<UTexture2D*> arrNormalImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_normal"); ///Check
	TArray<UTexture2D*> arrHoverImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_hover"); ///Check
	TArray<UTexture2D*> arrInfoImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "info_"); ///Check

	int hoverImgIndex = 0;
	int infoImgIndex = 0;

	for (int32 i = 0; i < arrNormalImgPtr.Num(); i++)
	{
		FInteriorSelectImage insInteriorSelectImg;
		
		insInteriorSelectImg.ImgNormalPtr = arrNormalImgPtr[i];

		NormalImgName = ExtractionStringByKeyword(arrNormalImgPtr[i]->GetName(), "inse_", "_normal");
		insInteriorSelectImg.LevelName = NormalImgName;

		if (hoverImgIndex < arrHoverImgPtr.Num())
		{
			HoverImgName = ExtractionStringByKeyword(arrHoverImgPtr[hoverImgIndex]->GetName(), "inse_", "_hover");
		}

		if (NormalImgName == HoverImgName)
		{
			insInteriorSelectImg.noRelease = false;
			insInteriorSelectImg.ImgHoverPtr = arrHoverImgPtr[hoverImgIndex];
			insInteriorSelectImg.LevelIndex = hoverImgIndex;

			hoverImgIndex++;
		}

		else
		{
			insInteriorSelectImg.noRelease = true;
			insInteriorSelectImg.ImgHoverPtr = arrNormalImgPtr[i];		// nullptr 방지
		}


		if (infoImgIndex < arrInfoImgPtr.Num() - 1)
		{
			InfoImgName = ExtractionStringByKeyword(arrInfoImgPtr[infoImgIndex]->GetName(), "info_", "");
		}

		if (NormalImgName == InfoImgName)
		{
			insInteriorSelectImg.ImgInfoPtr = arrInfoImgPtr[infoImgIndex];
			infoImgIndex++;
		}

		else
		{
			insInteriorSelectImg.ImgInfoPtr = arrNormalImgPtr[i];		// nullptr 방지
		}

		arrInteriorSelectImg.Add(insInteriorSelectImg);
	}
}

void InteriorInformations::SetLevelInformations()
{

}

void InteriorInformations::SetProductionState()
{
	//Add pre Setting		
	arrProductionState.Empty();

	int ActiveMapGap = 0;
	int InfoImgGap = 0;

	FString NormalImgName;
	FString HoverImgName;
	FString InfoImgName;

	TArray<UTexture2D*> arrNormalImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_normal"); ///Check
	TArray<UTexture2D*> arrHoverImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_hover"); ///Check
	TArray<UTexture2D*> arrInfoImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "info_"); ///Check

	for (int32 i = 0; i < arrNormalImgPtr.Num(); i++)
	{
		FProductionState insProductionState;

		NormalImgName = ExtractionStringByKeyword(arrNormalImgPtr[i]->GetName(), "inse_", "_normal");
		if (arrHoverImgPtr.Num() > i - ActiveMapGap) HoverImgName = ExtractionStringByKeyword(arrHoverImgPtr[i - ActiveMapGap]->GetName(), "inse_", "_hover");
		if (arrInfoImgPtr.Num() > i - InfoImgGap) InfoImgName = ExtractionStringByKeyword(arrInfoImgPtr[i - InfoImgGap]->GetName(), "info_", "");

		insProductionState.MapName = NormalImgName;
		insProductionState.MapAllNum = i;
		insProductionState.ImgNormalNum = i;
		insProductionState.ImgNormalPtr = arrNormalImgPtr[i];

		if (NormalImgName == HoverImgName)
		{
			insProductionState.isReleaseMap = true;
			insProductionState.MapReleaseNum = i - ActiveMapGap;
			insProductionState.ImgHoverNum = i - ActiveMapGap;
			insProductionState.ImgHoverPtr = arrHoverImgPtr[i - ActiveMapGap];
		}
		else
		{
			insProductionState.isReleaseMap = false;
			insProductionState.MapReleaseNum = -1;
			insProductionState.ImgHoverNum = -1;
			insProductionState.ImgHoverPtr = arrNormalImgPtr[i]; // prevent to Null pointer going to garbage
			ActiveMapGap++;
		}

		if (NormalImgName == InfoImgName)
		{
			insProductionState.ImgInfoNum = i - InfoImgGap;
			insProductionState.ImgInfoPtr = arrInfoImgPtr[i - InfoImgGap];
		}
		else
		{
			insProductionState.ImgInfoNum = -1;
			insProductionState.ImgInfoPtr = arrNormalImgPtr[i]; // prevent to Null pointer going to garbage
			InfoImgGap++;
		}
		arrProductionState.Add(insProductionState);
	}
}

//주어진 경로의 폴더 내에서 해당 키워드를 가진 asset파일을 찾아서 load하는 함수.(Texture)
TArray<UTexture2D*> InteriorInformations::DynamicLoadTextureWithKeyword(const FString& PathFromContentFolder, const FString& Keyword)
{
	TArray<UTexture2D*> arrTexturePtr;
	FString RootFolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + PathFromContentFolder + "/";

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> FileNames;
	FString SearchKey;

	if (!Keyword.Contains(TEXT("*")))
	{
		SearchKey = "*" + Keyword + "*.uasset";
	}
	else if (!Keyword.Contains(TEXT(".")))
	{
		SearchKey = Keyword + ".uasset";
	}
	else
	{
		SearchKey = Keyword;
	}

	UE_LOG(LogTemp, Log, TEXT("The Full File Serach Condition is : %s "), *(RootFolderFullPath + SearchKey));
	FileManager.FindFiles(FileNames, *(RootFolderFullPath + SearchKey), true, false);

	//파일이름을 다 알아냈으면 그 asset들의 포인터를 모으자.
	for (int32 index = 0; index < FileNames.Num(); index++)
	{
		//To do
		FString Path;
		Path = "/Game/" + PathFromContentFolder + "/" + FileNames[index].Replace(TEXT(".uasset"), TEXT(""));
		//UE_LOG(LogTemp, Log, TEXT("Path is : %s"), *Path);

		UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
		if (LoadedTexture != nullptr)
			arrTexturePtr.Add(LoadedTexture);
	}
	return arrTexturePtr;
}

FString InteriorInformations::ExtractionStringByKeyword(FString inSorce, const FString& prefix, const FString& suffix)
{
	FString sResult;
	FString Left, Right;
	FString FullFileName = inSorce;
	FullFileName.Split(prefix, &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	if (suffix != "")
	{
		Right.Split(suffix, &sResult, &Right, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	}
	else
	{
		sResult = Right;
	}
	return sResult;
}

FString InteriorInformations::LoadProjectConfig(const FString &config)
{
	bool bResult = false;

	TSharedPtr<FJsonObject> JsonToLoad;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(config);

	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad))
	{
		if (JsonToLoad->HasField("ProjectList"))
		{
			TArray<TSharedPtr<FJsonValue>> projectList = JsonToLoad->GetArrayField("ProjectList");

			// 첫번째 값을 디폴트로 씀
			TSharedPtr<FJsonObject> defaultProject = projectList.HeapTop().Get()->AsObject();
			return defaultProject->GetStringField("ProjectName");
		}
	}

	return "";
}

