// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimGameInstance.h"
#include "OlimBlueprintFunctionLibrary.h"
#include "OlimSaveGame.h"
#include "Engine.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Json.h"
#include "Misc/CommandLine.h"
#include "Http.h"
#include "StartConfigFunctionLibrary.h"
#include "InteriorInformations.h"
#include "LevelInformation.h"

#include "Kismet/GameplayStatics.h"

//static UEZipViewMode ZipviewMode = UEZipViewMode::ZV_HOME_MODE;
//static TArray<FName> MapFNames;

UOlimGameInstance::UOlimGameInstance()
{
	UE_LOG(LogTemp, Log, TEXT("Olim Game Instance constructor called."));

	BasicSet();

	CurrentZipViewMode = UEZipViewMode::ZV_HOME_MODE;
	CurrentInteriorMapNum = -1;
	AreaPageNum = 0;
	//CurrentArea360Num = -1;
	//CurrentArea3DNum = -1;

	MapFolder = "Unique/" + ZipViewID + "/3DResources/Maps"; ///Check
	MapNames.Empty();	
	AreaBtnInfo = LoadAreaTextData();
}


void UOlimGameInstance::BasicSet()
{
	FString InsParam;
	FString Path;

	//Login Token
	if (FParse::Value(FCommandLine::Get(), TEXT("ZipViewToken="), InsParam) == true)
	{
		ZipViewToken = InsParam;
	}
	else
	{
	}

	// Zipview ID
	if (FParse::Value(FCommandLine::Get(), TEXT("ZipviewID="), InsParam) == true)
	{
		ZipViewID = InsParam;
	}
	else
	{
		GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectName"), InsParam, GGameIni);
		if (InsParam.Find("_") != INDEX_NONE)
		{
			InsParam.Split("_", &ZipViewID, &Path, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		}
		else
		{
			ZipViewID = InsParam;
		}
	}

	FString ConfigRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../Config/", "ProjectConfig.json");

	if (!ConfigRaw.IsEmpty())
	{
		InsParam = LoadProjectConfig(ConfigRaw);

		if (!InsParam.IsEmpty())
		{
			ZipViewID = InsParam;
			UE_LOG(LogTemp, Log, TEXT("ProjectConfig : %s"), *ZipViewID);
		}

		else
		{
			UE_LOG(LogTemp, Log, TEXT("Parameter is empty"));
		}
	}

	else
	{
		UE_LOG(LogTemp, Log, TEXT("ConfigRaw is empty"));
	}

	// Confirm Certification
#if WITH_EDITOR
	ZipViewCertification = true;
#else
	ZipViewCertification = false;
#endif

	if (ZipViewCertification == false)
	{

	}
	// Confirm NetWork Connection	
}

void UOlimGameInstance::HttpGet(FString URL, FString Token)
{
	FString result;
	auto Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOlimGameInstance::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb("Get");
	Request->SetHeader(TEXT("x-user-auth-token"), Token);
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));

	Request->ProcessRequest();
}

void UOlimGameInstance::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString Result;
	int ResponseCode;

	Result = Response->GetContentAsString();
	ResponseCode = Response->GetResponseCode();

	if (ResponseCode == 200) ZipViewCertification = true;
}


void UOlimGameInstance::Init()
{
	Super::Init();	
	UE_LOG(LogTemp, Log, TEXT("Game Instance Init"));

	// Old ver
	/*
	//Add pre Setting		
	arrProductionState.Empty();

	int ActiveMapGap = 0;
	int InfoImgGap = 0;

	FString NormalImgName;
	FString HoverImgName;
	FString InfoImgName;

	TArray<UTexture2D*> arrNormalImgPtr = DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_normal"); ///Check
	TArray<UTexture2D*> arrHoverImgPtr	= DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "_hover"); ///Check
	TArray<UTexture2D*> arrInfoImgPtr	= DynamicLoadTextureWithKeyword("Unique/" + ZipViewID + "/2DResources/UIImage/03_InteriorSelect", "info_"); ///Check

	for (int32 i = 0; i < arrNormalImgPtr.Num(); i++)
	{
		FProductionState insProductionState;

		NormalImgName = ExtractionStringByKeyword(arrNormalImgPtr[i]->GetName(), "inse_", "_normal");
		if (arrHoverImgPtr.Num() > i - ActiveMapGap) HoverImgName = ExtractionStringByKeyword(arrHoverImgPtr[i - ActiveMapGap]->GetName(), "inse_", "_hover");
		if (arrInfoImgPtr.Num() > i - InfoImgGap) InfoImgName = ExtractionStringByKeyword(arrInfoImgPtr[i - InfoImgGap]->GetName(), "info_", "");

		//UE_LOG(LogTemp, Log, TEXT("Normal Image name is : %s "), *NormalImgName);
		//UE_LOG(LogTemp, Log, TEXT("Info Image name is : %s "), *InfoImgName);

		insProductionState.MapName	= NormalImgName;
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
	*/

	// New
	InteriorInformations* InteriorInfo = new InteriorInformations(ZipViewID);
	arrProductionState = InteriorInfo->arrProductionState;
	arrInteriorSelectImg = InteriorInfo->arrInteriorSelectImg;

	AllInteriorLevelsNum = arrProductionState.Num();

	delete InteriorInfo;
	InteriorInfo = 0;


	//CommonSetting.json Load
	if (!LoadCommonGameData())
	{		
		SaveCommonGameData();
		//LoadCommonGameData();		
	}


	//UniqueSetting.json Load
	if (!LoadUniqueGameData())
	{	
		SaveUniqueGameData();
		//LoadUniqueGameData();
	}
	
	UStartConfigFunctionLibrary::LoadConfig();


	AActor* od = UGameplayStatics::GetActorOfClass(GetWorld(), AOverrideData::StaticClass());
	AOverrideData* OverrideDataActor = Cast<AOverrideData>(od);
	if (OverrideDataActor != nullptr)
	{
		bOverride = true;
		OverrideData = OverrideDataActor->Data;
	}
}


int UOlimGameInstance::ProductionStateNum(const FString& CurrentMap)
{
	FString CurrentMapName, ins;
	CurrentMapName = CurrentMap;
	CurrentMapName.Split("_", &ins, &CurrentMapName, ESearchCase::IgnoreCase, ESearchDir::FromStart); ///check

	int length = arrProductionState.Num();

	for (int i = 0; i < length; i++)
	{		
		if (CurrentMapName == arrProductionState[i].MapName)
		{
			return i;
		}
		else continue;
	}
	return -1;
}


void UOlimGameInstance::PostInitProperties()
{
	Super::PostInitProperties();
	UE_LOG(LogTemp, Log, TEXT("PostInitProperties called"));

	//ReadMapNamesFromMapFolder(MapFolder, MapNames);

	LevelInformation* levelInfo = new LevelInformation(ZipViewID);
	
	MapNames = levelInfo->SetAllLevelsFromMapFolder();

	IsExteriorExist = levelInfo->IsExteriorExist;
	IsAreaExist = levelInfo->IsAreaExist;

	delete levelInfo;
	levelInfo = 0;
}

#if WITH_EDITOR
void UOlimGameInstance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//UE_LOG(LogTemp, Log, TEXT("PostEditChangeProperty called!"));	

	//ReadMapNamesFromMapFolder(MapFolder, MapNames);

	LevelInformation* levelInfo = new LevelInformation(ZipViewID);
	MapNames = levelInfo->SetAllLevelsFromMapFolder();

	IsExteriorExist = levelInfo->IsExteriorExist;
	IsAreaExist = levelInfo->IsAreaExist;

	delete levelInfo;
	levelInfo = 0;
}
#endif

void UOlimGameInstance::SettingGameMode()
{
	UE_LOG(LogTemp, Log, TEXT("Olim Game Instance constructor called"));
	//UGameInstance::OverrideGameModeClass();
	//AGameModeBase[] GameModeArray;	
	AGameModeBase ModeStart;
	AGameModeBase Mode360;
	AGameModeBase Mode3D;

	UWorld* World = GetWorld();
	AWorldSettings* WorldSettings = World->GetWorldSettings();
	TSubclassOf<AGameModeBase> ModeClass = WorldSettings->DefaultGameMode;
	
	TArray<UObject*> GameModeArray;
	
	GameModeArray = DynamicLoadObjectWithKeyword("COMMONS/Development/00_BlueprintMain", "Mode");
	UE_LOG(LogTemp, Log, TEXT("Custom GameMode Length is: %d "), GameModeArray.Num());
	for (int i = 0; i < GameModeArray.Num(); i++)
	{
		//if(GameModeArray[i].na)
	}
}



bool UOlimGameInstance::ReadMapNamesFromMapFolder(const FString& MapFolderPath, TArray<FName>& MapNameArrays)
{
	bool bResult = false;
	IsExteriorExist = false;
	IsAreaExist = false;

	FString MapFolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + MapFolderPath;
	//UE_LOG(LogTemp, Log, TEXT("The Full Path of Map Folder is : %s "), *MapFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> MapFiles;
	MapFiles.Empty();
	MapNameArrays.Empty();

	FileManager.FindFiles(MapFiles, *MapFolderFullPath, TEXT("umap"));

	for (int32 index = 0; index < MapFiles.Num(); index++)
	{
		if (!(MapFiles[index].Contains(ZipViewID + "_Exterior"))) ///check
		{
			FString strCleanFileName = MapFiles[index].Replace(TEXT(".umap"), TEXT(""));
			MapNameArrays.Add(FName(*strCleanFileName));
			UE_LOG(LogTemp, Log, TEXT("MapNameArrays added! : %s "), *strCleanFileName);
		}
		else
		{
			IsExteriorExist = true;
		}

		if (MapFiles[index].Contains(ZipViewID + "_Area")) IsAreaExist = true;
	}
	bResult = (MapNameArrays.Num() != 0) ? true : false;
	return bResult;
}

bool UOlimGameInstance::ReadAssetFileNamesFromFolder(const FString& FolderPath, const FString& KeyWord, TArray<FName>& FileNameArrays)
{
	bool bResult = false;
	FString FolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + FolderPath;

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> Files;
	Files.Empty();
	FileNameArrays.Empty();

	FileManager.FindFiles(Files, *FolderFullPath, TEXT("uasset"));

	for (int32 index = 0; index < Files.Num(); index++)
	{
		if (Files[index].Contains(KeyWord))
		{
			FString strCleanFileName = Files[index].Replace(TEXT(".uasset"), TEXT(""));
			FileNameArrays.Add(FName(*strCleanFileName));
			bResult = true;
		}
	}
	return bResult;
}


TArray<UObject*> UOlimGameInstance::DynamicLoadObjFromPath(const FString& PathFromContentFolder, UClass* AssetClass)
{
	TArray<UObject*> arrObjectPtr;
	FString RootFolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + PathFromContentFolder;
	UE_LOG(LogTemp, Log, TEXT("The Full Path of destination Folder is : %s "), *RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> FileNames;

	FileManager.FindFiles(FileNames, *RootFolderFullPath, TEXT("uasset"));

	for (int32 index = 0; index < FileNames.Num(); index++)
	{
		//To do
		FString Path;
		Path = "/Game/" + PathFromContentFolder + "/" + FileNames[index].Replace(TEXT(".uasset"), TEXT(""));
		//UE_LOG(LogTemp, Log, TEXT("Path is : %s"), *Path);

		UObject* LoadedObj = StaticLoadObject(AssetClass, NULL, *Path);
		arrObjectPtr.Add(LoadedObj);
	}	
	return arrObjectPtr;
}

TArray<UTexture2D*> UOlimGameInstance::DynamicLoadTexture2DFromPath(const FString& PathFromContentFolder)
{
	TArray<UTexture2D*> arrTexturePtr;
	TArray<UObject*> arrObj = DynamicLoadObjFromPath(PathFromContentFolder, UTexture2D::StaticClass());

	for (int32 i = 0; i < arrObj.Num(); i++)
	{
		UTexture2D* ptrTexture2D = Cast<UTexture2D>(arrObj[i]);
		arrTexturePtr.Add(ptrTexture2D);
	}
	return arrTexturePtr;
}

#include "Engine/AssetManager.h"

TArray<UTexture2D*> UOlimGameInstance::DynamicLoadTextureWithKeyword(const FString& PathFromContentFolder, const FString& Keyword)
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
	
	// for (int32 index = 0; index < FileNames.Num(); index++)
	// {
		// //To do
		// FString Path;
		// Path = "/Game/" + PathFromContentFolder + "/" + FileNames[index].Replace(TEXT(".uasset"), TEXT(""));
		// //UE_LOG(LogTemp, Log, TEXT("Path is : %s"), *Path);

		// FSoftObjectPath SOPath = FString(Path);
		// //Note: before the resource is loaded, the code will pause in this line to wait for the resource to load.
		// TSharedPtr<FStreamableHandle> SyncStreamableHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(SOPath);
		// if (SyncStreamableHandle)
		// {
			// UTexture2D * UImg2D = Cast<UTexture2D>(SyncStreamableHandle->GetLoadedAsset());
			// if (UImg2D)
			// {
				// UE_LOG(LogTemp, Log, TEXT("%s is loaded(%d x %d) (%d x %d)"), *Path, UImg2D->GetSizeX(), UImg2D->GetSizeY(), UImg2D->GetSurfaceWidth(), UImg2D->GetSurfaceHeight());
				// arrTexturePtr.Add(UImg2D);
			// }
		// }
	// }
	
	return arrTexturePtr;
}

TArray<UObject*> UOlimGameInstance::DynamicLoadObjectWithKeyword(const FString& PathFromContentFolder, const FString& Keyword)
{
	TArray<UObject*> arrObjectPtr;
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

	//UE_LOG(LogTemp, Log, TEXT("The Full File Serach Condition is : %s "), *(RootFolderFullPath + SearchKey));
	FileManager.FindFiles(FileNames, *(RootFolderFullPath + SearchKey), true, false);

	for (int32 index = 0; index < FileNames.Num(); index++)
	{
		//To do
		FString Path;
		Path = "/Game/" + PathFromContentFolder + "/" + FileNames[index].Replace(TEXT(".uasset"), TEXT(""));
		//UE_LOG(LogTemp, Log, TEXT("Path is : %s"), *Path);

		UObject* LoadedObject = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *Path));
		if (LoadedObject != nullptr)
			arrObjectPtr.Add(LoadedObject);
	}
		
	return arrObjectPtr;
}


TArray<UTexture2D*> UOlimGameInstance::DynamicLoadTextureWithMapAndKeyword(const FString& PathFromContentFolder, const FString& prefix, const FString& suffix)
{
	TArray<UTexture2D*> arrTexturePtr;

	for (int32 i = 0; i < MapNames.Num(); i++)
	{
		FString Path = "/Game/" + PathFromContentFolder + "/" + prefix + MapNames[i].ToString() + suffix;
		//UE_LOG(LogTemp, Log, TEXT("Combined Path of Map is : %s"), *Path);

		UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
		if (LoadedTexture != nullptr)
			arrTexturePtr.Add(LoadedTexture);
	}
	for (int32 i = 0; i < arrTexturePtr.Num(); i++)
	{
		if (arrTexturePtr[i] != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Element at %i is %s"), i, *(arrTexturePtr[i]->GetFName().ToString()));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Element at %i is NULL!!!"), i);
		}
	}
	return arrTexturePtr;
}


FString UOlimGameInstance::ExtractionStringByKeyword(FString inSorce, const FString& prefix, const FString& suffix)
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


bool UOlimGameInstance::SaveCommonGameData()
{
	bool bResult = false;
	TSharedPtr<FJsonObject> JsonToSave = MakeShareable(new FJsonObject);
		
	JsonToSave->SetBoolField("bTotalAutoMode", bTotalAutoMode);
	JsonToSave->SetBoolField("bFullHmdMode", bFullHmdMode);
	JsonToSave->SetBoolField("bExteriorMenuNotShow", bExteriorMenuNotShow);
	JsonToSave->SetBoolField("bInteriorMenuNotShow", bInteriorMenuNotShow);
	JsonToSave->SetBoolField("bEstateMenuNotShow", bEstateMenuNotShow);
	JsonToSave->SetBoolField("bMapMenuNotShow", bMapMenuNotShow);
	JsonToSave->SetBoolField("bConditionMenuNotShow", bConditionMenuNotShow);
	JsonToSave->SetBoolField("bGuideBookMenuNotShow", bGuideBookMenuNotShow);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonToSave.ToSharedRef(), JsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../../../ContentSet/", "CommonSetting.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../../ContentSet/", "CommonSetting.json");
#endif
	return bResult;
}


bool UOlimGameInstance::LoadCommonGameData()
{
	bool bResult = false;

#if UE_BUILD_SHIPPING
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../../../ContentSet/", "CommonSetting.json");
#else
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../../ContentSet/", "CommonSetting.json");
#endif
	
	if (JsonRaw.IsEmpty())
	{
		bTotalAutoMode			= false;	
		bFullHmdMode			= false;
		bExteriorMenuNotShow	= false;
		bInteriorMenuNotShow	= false;
		bEstateMenuNotShow		= false;
		bMapMenuNotShow			= false;
		bConditionMenuNotShow	= false;
		bGuideBookMenuNotShow	= false;
		return false;
	}
	
	TSharedPtr<FJsonObject> JsonToLoad;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);
	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad))
	{
		bResult = true;

		if (JsonToLoad->HasField("bTotalAutoMode")) bTotalAutoMode = JsonToLoad->GetBoolField("bTotalAutoMode");
		else 
		{
			bTotalAutoMode = false;
			bResult = false;
		}	

		if (JsonToLoad->HasField("bFullHmdMode")) bFullHmdMode = JsonToLoad->GetBoolField("bFullHmdMode");
		else
		{
			bFullHmdMode = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bExteriorMenuNotShow")) bExteriorMenuNotShow = JsonToLoad->GetBoolField("bExteriorMenuNotShow");
		else
		{
			bExteriorMenuNotShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bInteriorMenuNotShow")) bInteriorMenuNotShow = JsonToLoad->GetBoolField("bInteriorMenuNotShow");
		else
		{
			bInteriorMenuNotShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bEstateMenuNotShow")) bEstateMenuNotShow = JsonToLoad->GetBoolField("bEstateMenuNotShow");
		else
		{
			bEstateMenuNotShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bMapMenuNotShow")) bMapMenuNotShow = JsonToLoad->GetBoolField("bMapMenuNotShow");
		else
		{
			bMapMenuNotShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bConditionMenuNotShow")) bConditionMenuNotShow = JsonToLoad->GetBoolField("bConditionMenuNotShow");
		else
		{
			bConditionMenuNotShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bGuideBookMenuNotShow")) bGuideBookMenuNotShow = JsonToLoad->GetBoolField("bGuideBookMenuNotShow");
		else
		{
			bGuideBookMenuNotShow = false;
			bResult = false;
		}
	}
	return bResult;
}

bool UOlimGameInstance::SaveUniqueGameData()
{
	bool bResult = false;
	TSharedPtr<FJsonObject> JsonToSave = MakeShareable(new FJsonObject);

	JsonToSave->SetBoolField("bLowPerformance", bLowPerformance);
	JsonToSave->SetBoolField("bInfoExtractMode", bInfoExtractMode);

	JsonToSave->SetBoolField("bZipviewEnterPrise", bZipviewEnterPrise);

	JsonToSave->SetBoolField("bProductionInfoShow", bProductionInfoShow);
	JsonToSave->SetBoolField("bGuideBookPDFShow", bGuideBookPDFShow);

	JsonToSave->SetStringField("strExteriorInfoMsg", strExteriorInfoMsg);
	JsonToSave->SetStringField("strInteriorInfoMsg", strInteriorInfoMsg);
	JsonToSave->SetStringField("strAreaInfoMsg", strAreaInfoMsg);
	JsonToSave->SetStringField("strMapURL", strMapURL);
	JsonToSave->SetStringField("strConditionURL", strConditionURL);
	JsonToSave->SetStringField("MainColorString", MainColorString);
	JsonToSave->SetStringField("SubColorString", SubColorString);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonToSave.ToSharedRef(), JsonWriter);

	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "Unique/" + ZipViewID + "/Development/03_ETC", "UniqueSetting.json"); ///Check

	return bResult;
}


bool UOlimGameInstance::LoadUniqueGameData()
{
	bool bResult = false;	
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipViewID + "/Development/03_ETC", "UniqueSetting.json"); ///Check

	if (JsonRaw.IsEmpty())
	{
		bLowPerformance			= false;
		bInfoExtractMode		= false;
		bZipviewEnterPrise		= false;
		bProductionInfoShow		= false;
		bGuideBookPDFShow		= false;
		strExteriorInfoMsg		= UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "ExteriorGuide.txt");
		strInteriorInfoMsg		= UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "InteriorGuide.txt");
		strAreaInfoMsg			= UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "AreaGuide.txt");
		strMapURL				= "";
		strConditionURL			= "";
		MainColorString			= "218/55/33";
		SubColorString			= "255/255/255";
		return false;
	}
	
	TSharedPtr<FJsonObject> JsonToLoad;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);
	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad))
	{
		bResult = true;

		if (JsonToLoad->HasField("bLowPerformance")) bLowPerformance = JsonToLoad->GetBoolField("bLowPerformance");
		else
		{
			bLowPerformance = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bInfoExtractMode")) bInfoExtractMode = JsonToLoad->GetBoolField("bInfoExtractMode");
		else 
		{
			bInfoExtractMode = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bZipviewEnterPrise")) bZipviewEnterPrise = JsonToLoad->GetBoolField("bZipviewEnterPrise");
		else
		{
			bZipviewEnterPrise = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bProductionInfoShow")) bProductionInfoShow = JsonToLoad->GetBoolField("bProductionInfoShow");
		else 
		{
			bProductionInfoShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("bGuideBookPDFShow")) bGuideBookPDFShow = JsonToLoad->GetBoolField("bGuideBookPDFShow");
		else 
		{
			bGuideBookPDFShow = false;
			bResult = false;
		}

		if (JsonToLoad->HasField("strExteriorInfoMsg")) strExteriorInfoMsg = JsonToLoad->GetStringField("strExteriorInfoMsg");
		else
		{
			strExteriorInfoMsg = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "ExteriorGuide.txt");
			bResult = false;
		}

		if (JsonToLoad->HasField("strInteriorInfoMsg")) strInteriorInfoMsg = JsonToLoad->GetStringField("strInteriorInfoMsg");
		else 
		{
			strInteriorInfoMsg = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "InteriorGuide.txt");
			bResult = false;
		}

		if (JsonToLoad->HasField("strAreaInfoMsg")) strAreaInfoMsg = JsonToLoad->GetStringField("strAreaInfoMsg");
		else 
		{
			strAreaInfoMsg = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Text", "AreaGuide.txt");
			bResult = false;
		}

		if (JsonToLoad->HasField("strMapURL")) strMapURL = JsonToLoad->GetStringField("strMapURL");
		else 
		{
			strMapURL = "";
			bResult = false;
		}

		if (JsonToLoad->HasField("strConditionURL")) strConditionURL = JsonToLoad->GetStringField("strConditionURL");
		else 
		{
			strConditionURL = "";
			bResult = false;
		}

		if (JsonToLoad->HasField("MainColorString")) MainColorString = JsonToLoad->GetStringField("MainColorString");
		else
		{
			MainColorString = "218/55/33";
			bResult = false;
		}

		if (JsonToLoad->HasField("SubColorString")) SubColorString = JsonToLoad->GetStringField("SubColorString");
		else
		{
			SubColorString = "255/255/255";
			bResult = false;
		}
	}
	return bResult;
}

FString UOlimGameInstance::LoadAreaTextData()
{
	FString FileData;
	FString TextRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("Unique/" + ZipViewID + "/Development/03_ETC", "AreaBtnData.txt"); ///Check

	if (TextRaw.IsEmpty())
	{
		FileData = "None";
	}
	else
	{
		FString FilePath = FPaths::ProjectDir() + "Content/Unique/" + ZipViewID + "/Development/03_ETC/AreaBtnData.txt";
		FFileHelper::LoadFileToString(FileData, *FilePath);
	}
	return FileData;
}

FString UOlimGameInstance::LoadProjectConfig(const FString &config)
{
	bool bResult = false;

	TSharedPtr<FJsonObject> JsonToLoad;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(config);

	if (FJsonSerializer::Deserialize(JsonReader, JsonToLoad))
	{
		if (JsonToLoad->HasField("ProjectList"))
		{
			TArray<TSharedPtr<FJsonValue>> projectList = JsonToLoad->GetArrayField("ProjectList");

			TSharedPtr<FJsonObject> defaultProject = projectList.HeapTop().Get()->AsObject();
			return defaultProject->GetStringField("ProjectName");
		}
	}

	return "";
}


