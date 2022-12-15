// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimWebConvertFunction.h"
#include "OlimBlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"
#include "OlimBlueprintFunctionLibrary.h"
#include "ObjectInfoIndicatorBase.h"
#include "EngineUtils.h"

bool UOlimWebConvertFunction::JsonInfoFileCheck(const FString& mapName, FString& fileName)
{
	bool bResult = false;

	FString ContentDirectory = FPaths::ProjectContentDir();
	FString FileName, insFileName, insMapName;

	ContentDirectory.Split("/Content", &FileName, &insFileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	FileName.Split("/", &insFileName, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	fileName = FileName;

	FString ExtraPath;
	if (mapName == "Exterior")
	{
		ExtraPath = mapName + "/";
	}
	else
	{
		ExtraPath = "Interior/" + mapName + "/";
	}

#if UE_BUILD_SHIPPING
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../../web360Info/" + ExtraPath, "data.json");
#else
	FString JsonRaw = UOlimBlueprintFunctionLibrary::LoadStringFromFile("../web360Info/" + ExtraPath, "data.json");
#endif

	if (JsonRaw.IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
	/*
	if (FPaths::DirectoryExists(ContentDirectory))
	{
		// Set FileName
		if (FPaths::FileExists(ContentDirectory + "/" + FileName + ".json"))
		{
			return false;
		}
	}
	*/
	return bResult;
}

bool UOlimWebConvertFunction::InteriorArrInfoGen(const FString& fileName, const FString& levelName, const FString& TypeBtnName, const TArray<FString>& minimapName,
	const TArray<ATargetPoint*> targetGath, const TArray<FVector2D>& scaleCorr, const TArray<FVector2D>& posCorr, const bool& isMultiFloor, const bool& isReverse, const int& ins2ndStartNum)
{
	const int Version = 2;
	bool bResult = false;

	TSharedPtr<FJsonObject> _jResult = MakeShareable(new FJsonObject);

	_jResult->SetNumberField("version", Version);

	for (int i = 0; i < minimapName.Num(); i++)
	{
		if (i == 0)
		{
			FString fstr = minimapName[i] + ".png";
			_jResult->SetStringField("minimapName", fstr);
		}
		else if (i == 1)
		{
			FString fstr = minimapName[i] + ".png";
			_jResult->SetStringField("minimapName_2f", fstr);
		}
		else if (i == 2)
		{
			FString fstr = minimapName[i] + ".png";
			_jResult->SetStringField("minimapName_3f", fstr);
		}
	}
	FString fstrBtnName = TypeBtnName + ".png";
	_jResult->SetStringField("typeBtnName", fstrBtnName);

	auto target_list = targetGath;
	auto filtered = target_list.FilterByPredicate([](ATargetPoint* tp) {
		return tp != nullptr;
	});

	TArray<TSharedPtr<FJsonValue>> _jPointArray;
	for (int i = 0; i < filtered.Num(); i++)
	{
		auto p360 = filtered[i];
		TSharedPtr<FJsonObject> _jPoint = MakeShareable(new FJsonObject);
		_jPointArray.Add(MakeShareable(new FJsonValueObject(_jPoint)));
		int insArrNum;

		if (isMultiFloor == false)
		{
			insArrNum = 0;
		}
		else if (scaleCorr.Num() == 1)
		{
			insArrNum = 0;
		}
		else
		{
			if (i >= ins2ndStartNum)
			{
				if (isReverse == false)
				{
					insArrNum = 1;
				}
				else
				{
					insArrNum = 0;
				}
			}
			else
			{
				if (isReverse == false)
				{
					insArrNum = 0;
				}
				else
				{
					insArrNum = 1;
				}
			}
		}

		auto sc = scaleCorr[insArrNum];
		auto pc = posCorr[insArrNum];
		if (sc.X == 0.f) sc.X = 1.f;
		if (sc.Y == 0.f) sc.Y = 1.f;
		if (pc.X == 0.f) pc.X = 1.f;
		if (pc.Y == 0.f) pc.Y = 1.f;

		float posCorrX = ((p360->GetActorLocation().X / sc.X) + pc.X + 160) * 0.9657;
		float posCorrY = (p360->GetActorLocation().Y / sc.Y) + pc.Y + 153;

		int floorCorr = 1;
		if (p360->GetActorLocation().Z >= 200)
		{
			floorCorr = 2;
		}

		FString TargetName, TargetNameIns;
		p360->GetName().Split("_", &TargetName, &TargetNameIns, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		FString TargetNum = FString::FromInt(i);

		if (i < 10)
		{
			TargetNum = "0" + TargetNum;
		}

		FString s;
		s = fileName + "__" + levelName + "__360Point_" + TargetNum + ".jpg";

		_jPoint->SetStringField("name", s);
		_jPoint->SetNumberField("x", posCorrX);
		_jPoint->SetNumberField("y", posCorrY);
		_jPoint->SetNumberField("floor", floorCorr);
		_jPoint->SetNumberField("dir", DegreeToRad(p360->GetActorRotation().Yaw + 90));
		TArray<AActor*> InfoList;
		TArray<TSharedPtr<FJsonValue>> _jInfoList;
		UGameplayStatics::GetAllActorsOfClass(p360->GetWorld(), AObjectInfoIndicatorBase::StaticClass(), InfoList);

		for (const auto& pActor : InfoList)
		{
			if (auto pBase = Cast<AObjectInfoIndicatorBase>(pActor))
			{
				if (pBase->IsValidTargetPoint(p360) == true)
				{
					TSharedPtr<FJsonObject> _jInfo = MakeShareable(new FJsonObject);
					_jInfoList.Add(MakeShareable(new FJsonValueObject(_jInfo)));

					{
						auto BaseLocation = pBase->GetActorTransform().GetLocation();
						auto p360Location = p360->GetActorTransform().GetLocation();
						//auto Camera = p360->GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
						//auto CameraLocation = Camera->GetCameraLocation();
						//p360Location.Z = CameraLocation.Z;
						auto SubLocation2 = BaseLocation - p360Location;
						SubLocation2.Normalize();
						auto dirRot2 = SubLocation2.Rotation();
						auto pointRot2 = p360->GetActorTransform().GetRotation().GetForwardVector().Rotation();
						auto diffYaw2 = dirRot2.Yaw - pointRot2.Yaw;
						auto diffPitch2 = dirRot2.Pitch - pointRot2.Pitch;
						_jInfo->SetNumberField("yaw", DegreeToRad(diffYaw2));
						_jInfo->SetNumberField("pitch", -DegreeToRad(diffPitch2));
					}

					if (pBase->TextLabel.IsEmpty() == false)
					{
						_jInfo->SetStringField("option", pBase->TextLabel);
					}

					FString TempIconID;
					TArray<TSharedPtr<FJsonValue>> _jDataList;
					switch (pBase->Type)
					{
					case ObjectInfoType::IMAGE:
						_jInfo->SetStringField("type", "Image");
						TempIconID = "image";
						for (const auto& sl : pBase->ImageList)
						{
							FString fstrPathName = sl.GetResourceName().ToString();
							fstrPathName += ".jpg";
							_jDataList.Add(MakeShareable(new FJsonValueString(fstrPathName)));
						}
						break;
					case ObjectInfoType::VIDEO_URL:
						_jInfo->SetStringField("type", "VideoUrl");
						TempIconID = "video";
						for (const auto& mu : pBase->MediaUrlList)
						{
							FString fstr = mu;
							int32 FindIndex = 0;
							if (fstr.FindChar('/', FindIndex) == false)
							{
								fstr = "https://youtube.com/embed/" + fstr + "?autoplay=1";
							}
							_jDataList.Add(MakeShareable(new FJsonValueString(fstr)));
						}
						break;
					case ObjectInfoType::WEB_PAGE:
					case ObjectInfoType::OS_BROWSER:
						_jInfo->SetStringField("type", "WebPage");
						TempIconID = "info";
						for (const auto& mu : pBase->WebPageUrlList)
						{
							_jDataList.Add(MakeShareable(new FJsonValueString(mu)));
						}
						break;
					case ObjectInfoType::MODEL_3D:
						_jInfo->SetStringField("type", "Model3D");
						TempIconID = "model";
						for (const auto& sm : pBase->StaticMeshList)
						{
							_jDataList.Add(MakeShareable(new FJsonValueString(sm.TargetURL)));
						}
						break;
					case ObjectInfoType::CHATTING:
						_jInfo->SetStringField("type", "Chatting");
						TempIconID = "chatting";
						//for (const auto& mu : pBase->WebPageUrlList)
						//{
						//	_jDataList.Add(MakeShareable(new FJsonValueString(mu)));
						//}
						break;
					case ObjectInfoType::NONE:
						_jInfo->SetStringField("type", "None");
						break;
					default:
						break;
					}

					if (pBase->IconID.IsEmpty() == false)
						TempIconID = pBase->IconID;

					_jInfo->SetStringField("icon", TempIconID.ToLower());

					if (_jDataList.Num() != 0)
						_jInfo->SetArrayField("dataList", _jDataList);
				}
			}
		}


		if (_jInfoList.Num() != 0)
			_jPoint->SetArrayField("infoList", _jInfoList);
		_jResult->SetArrayField("pointInfo", _jPointArray);
	}

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	FJsonSerializer::Serialize(_jResult.ToSharedRef(), JsonWriter);
#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../../web360Info/interior/" + levelName + "/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(OutputString, "../web360Info/interior/" + levelName + "/", "data.json");
#endif

	return bResult;
}

bool UOlimWebConvertFunction::Interior360ArrInfoGen(const FString& fileName, const FString& levelName, const FString& TypeBtnName, const TArray<FString>& minimapName, const TArray<FString>& imageName360, const TArray<FVector2D>& minimapPoint, const TArray<float>& direction, const TArray<float>& angle)
{
	bool bResult = false;

	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	TSharedPtr<FJsonObject> jsonObj;

	float posCorrX, posCorrY;
	int floorCorr;
	FString outputString;

	jsonValArr.Empty();

	for (int i = 0; i < imageName360.Num(); i++)
	{
		jsonObj = MakeShareable(new FJsonObject);

		floorCorr = 1;
		posCorrX = (minimapPoint[i].X + 160)*0.9657;
		posCorrY = minimapPoint[i].Y + 153;

		jsonObj->SetStringField("name", imageName360[i] + ".jpg");
		jsonObj->SetNumberField("x", posCorrX);
		jsonObj->SetNumberField("y", posCorrY);
		jsonObj->SetNumberField("floor", floorCorr);
		jsonObj->SetNumberField("dir", DegreeToRad(angle[i] - 90));

		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
		jsonValArr.Add(jsonValue);
	}
	TSharedPtr<FJsonObject> jsonObjArr = MakeShareable(new FJsonObject);

	jsonObjArr->SetStringField("minimapName", minimapName[0] + ".png");
	jsonObjArr->SetStringField("typeBtnName", TypeBtnName + ".png");
	jsonObjArr->SetArrayField("pointInfo", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObjArr.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/interior/" + levelName + "/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/interior/" + levelName + "/", "data.json");
#endif

	return bResult;
}

bool UOlimWebConvertFunction::InteriorBtnInfoGen(const FString& fileName, const int& length, const TArray<FString>& mapNameArr, const TArray<FString>& btnImageNameArr, const TArray<FString>& btn3dModeArr, const TArray<FString>& DpName)
{
	bool bResult = false;
	FString outputString;

	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> jsonValArr;

	for (int i = 0; i < length; i++)
	{
		TSharedPtr<FJsonObject> jsonInsObj = MakeShareable(new FJsonObject);
		jsonInsObj->SetStringField("type", mapNameArr[i]);
		jsonInsObj->SetStringField("image", btnImageNameArr[i] + ".png");
		jsonInsObj->SetStringField("mode", btn3dModeArr[i]);
		jsonInsObj->SetStringField("option", DpName[i]);
		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonInsObj));
		jsonValArr.Add(jsonValue);
	}

	jsonObj->SetArrayField("selectButtons", jsonValArr);
	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/interior/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/interior/", "data.json");
#endif

	return bResult;
}


bool UOlimWebConvertFunction::IsoArrInfoGen(const FString& fileName, const FString& levelName, const int& AddAngle)
{
	bool bResult = false;

	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	TSharedPtr<FJsonObject> jsonObj;
	FString outputString;

	jsonValArr.Empty();

	for (int i = 0; i*AddAngle < 360; i++)
	{
		jsonObj = MakeShareable(new FJsonObject);

		FString TargetNum = FString::FromInt(i);
		if (i < 10) TargetNum = "0" + TargetNum;

		jsonObj->SetStringField("name", fileName + "__" + levelName + "__Iso_" + TargetNum + ".jpg");

		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
		jsonValArr.Add(jsonValue);
	}
	TSharedPtr<FJsonObject> jsonObjArr = MakeShareable(new FJsonObject);
	jsonObjArr->SetArrayField("pointInfo", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObjArr.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/interior/" + levelName + "/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/interior/" + levelName + "/", "data.json");
#endif

	return bResult;
}


bool UOlimWebConvertFunction::IsoWebImgGen(const FString& fileName, const FString& levelName, const int& index)
{
	bool bResult = false;
	FString ImgName, ImgNum, SaveFolderPath, FullPathFromRoot;

	ImgNum = FString::FromInt(index);
	if (index < 10) ImgNum = "0" + ImgNum;

	ImgName = fileName + "__" + levelName + "__Iso_" + ImgNum + ".jpg";


#if UE_BUILD_SHIPPING
	SaveFolderPath = "../../web360Info/interior/" + levelName + "/";
#else
	SaveFolderPath = "../web360Info/interior/" + levelName + "/";
#endif
	FullPathFromRoot = UOlimBlueprintFunctionLibrary::ConvertRelativePathToAbsolutePath(SaveFolderPath) + ImgName;

	FScreenshotRequest::RequestScreenshot(FullPathFromRoot, false, false);
	bResult = FScreenshotRequest::IsScreenshotRequested();

	return bResult;
}


bool UOlimWebConvertFunction::ExteriorArrInfoGen(const FString& fileName, const FString& levelName, const TArray<UTexture*> imageArr, const TArray<FRotator>& defaltRot,
	const TArray<float>& arrowAngle, const TArray<int>& iconNum, const TArray<int>& iconInfo, const TArray<FString>& iconName, const TArray<FVector>& iconPos)
{
	bool bResult = false;
	FString outputString;

	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	jsonValArr.Empty();

	TSharedPtr<FJsonObject> jsonTotalObj = MakeShareable(new FJsonObject);
	TSharedPtr<FJsonObject> jsonObj;
	TSharedPtr<FJsonObject> jsonInsObj;

	TSharedPtr<FJsonObject> jsonInfoObj;
	TArray<TSharedPtr<FJsonValue>> jsonInfoBasic;
	TArray<TSharedPtr<FJsonValue>> jsonInfoTraffic;
	TArray<TSharedPtr<FJsonValue>> jsonInfoFacilities;
	TArray<TSharedPtr<FJsonValue>> jsonInfoEducation;
	TArray<TSharedPtr<FJsonValue>> jsonInfoNature;
	TArray<TSharedPtr<FJsonValue>> jsonInfoEstate;

	for (int i = 0; i < imageArr.Num(); i++)
	{
		jsonObj = MakeShareable(new FJsonObject);
		jsonInsObj = MakeShareable(new FJsonObject);

		if (imageArr[i] != NULL)
		{
			jsonObj->SetStringField("name", imageArr[i]->GetName() + ".jpg");

			jsonInsObj->SetNumberField("pitch", DegreeToRad(-defaltRot[i].Pitch));
			jsonInsObj->SetNumberField("yaw", DegreeToRad(defaltRot[i].Yaw + 90) + 0.3);
			jsonInsObj->SetNumberField("compass", DegreeToRad(arrowAngle[i] + 90));
			jsonInsObj->SetNumberField("fov", 1.5707963267948966);
		}
		TSharedPtr<FJsonValue> jsonInsVal = MakeShareable(new FJsonValueObject(jsonInsObj));
		jsonObj->SetField("initialParameters", jsonInsVal);

		jsonInfoBasic.Empty();
		jsonInfoTraffic.Empty();
		jsonInfoFacilities.Empty();
		jsonInfoEducation.Empty();
		jsonInfoNature.Empty();
		jsonInfoEstate.Empty();

		for (int u = 0; u < iconNum.Num(); u++)
		{
			if (iconNum[u] == i)
			{
				jsonInfoObj = MakeShareable(new FJsonObject);
				jsonInfoObj->SetStringField("name", iconName[u] + ".png");

				FVector2D Pos = PosToRad(FVector(0, 0, 0), iconPos[u]);
				jsonInfoObj->SetNumberField("pitch", -Pos.X - 0.02);
				jsonInfoObj->SetNumberField("yaw", Pos.Y + 1.57077 + 0.3);

				TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonInfoObj));

				if (iconInfo[u] == 0) jsonInfoBasic.Add(jsonValue);
				else if (iconInfo[u] == 1) jsonInfoTraffic.Add(jsonValue);
				else if (iconInfo[u] == 2) jsonInfoFacilities.Add(jsonValue);
				else if (iconInfo[u] == 3) jsonInfoEducation.Add(jsonValue);
				else if (iconInfo[u] == 4) jsonInfoNature.Add(jsonValue);
				else if (iconInfo[u] == 5) jsonInfoEstate.Add(jsonValue);
			}
		}
		jsonObj->SetArrayField("basicHotspots", jsonInfoBasic);
		jsonObj->SetArrayField("trafficHotspots", jsonInfoTraffic);
		jsonObj->SetArrayField("facilitiesHotspots", jsonInfoFacilities);
		jsonObj->SetArrayField("educationHotspots", jsonInfoEducation);
		jsonObj->SetArrayField("natureHotspots", jsonInfoNature);
		jsonObj->SetArrayField("estateHotspots", jsonInfoEstate);

		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
		jsonValArr.Add(jsonValue);
	}

	jsonTotalObj->SetArrayField("scenes", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonTotalObj.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/exterior/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/exterior/", "data.json");
#endif

	return bResult;
}


bool UOlimWebConvertFunction::AreaArrInfoGen(const FString& fileName, const FString& Area360LevelName, const FString& Area3DLevelName, const TArray<FString>& AreaSelectorBgName,
	const TArray<float>& PosX, const TArray<float>& PosY, const TArray<FString>& Type, const TArray<FString>& Src, const TArray<int>& PageNum)
{
	bool bResult = false;
	FString outputString;

	TSharedPtr<FJsonObject> jsonInfoObj = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	jsonValArr.Empty();

	for (int u = 0; u < AreaSelectorBgName.Num(); u++)
	{
		TArray<TSharedPtr<FJsonValue>> jsonInsArr;
		for (int i = 0; i < PosX.Num(); i++)
		{
			if (PageNum[i] == u)
			{
				TSharedPtr<FJsonObject> jsonInsObj = MakeShareable(new FJsonObject);
				jsonInsObj->SetNumberField("x", PosX[i]);
				jsonInsObj->SetNumberField("y", PosY[i]);
				jsonInsObj->SetStringField("type", Type[i]);

				if (Type[i] == "image" && Src[i].Contains(","))
				{
					TArray<FString> insSrc;
					FString insString = Src[i];
					FString Left, Right;

					while (insString.Split(",", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromStart))
					{
						insSrc.Add(Left);
						insString = Right;
					}
					insSrc.Add(insString);

					TArray<TSharedPtr<FJsonValue>> jsonValArr2;
					for (int s = 0; s < insSrc.Num(); s++)
					{
						//FString insString2 = FString::FromInt(s);
						//TSharedPtr<FJsonObject> jsonInsObj3 = MakeShareable(new FJsonObject);
						//jsonInsObj3->SetStringField(insString2, insSrc[s]);
						//TSharedRef<FJsonValueObject> jsonValue2 = MakeShareable(new FJsonValueObject(jsonInsObj3));
						//jsonValArr2.Add(jsonValue2);
						TSharedPtr<FJsonValueString> jsonInsVal3 = MakeShareable(new FJsonValueString(insSrc[s]));
						jsonValArr2.Add(jsonInsVal3);
					}
					jsonInsObj->SetArrayField("src", jsonValArr2);
				}
				else if (Type[i] == "tour" && Src[i].Contains("360"))
				{
					FString Left2, Right2;
					Src[i].Split("_", &Left2, &Right2, ESearchCase::IgnoreCase, ESearchDir::FromStart);
					jsonInsObj->SetStringField("src", Left2);
					jsonInsObj->SetStringField("target", Right2);
				}
				else
				{
					jsonInsObj->SetStringField("src", Src[i]);
				}
				TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonInsObj));
				jsonInsArr.Add(jsonValue);
			}
		}

		TSharedPtr<FJsonObject> jsonInsObj2 = MakeShareable(new FJsonObject);
		jsonInsObj2->SetStringField("image", AreaSelectorBgName[u]);
		jsonInsObj2->SetArrayField("points", jsonInsArr);

		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonInsObj2));
		jsonValArr.Add(jsonValue);
	}

	jsonInfoObj->SetStringField("type", "map");
	jsonInfoObj->SetArrayField("data", jsonValArr);

	FString FilePath;
	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonInfoObj.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/estate/", "data.json");
	//FilePath = "../../web360Info/" + fileName + "/area/";
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/estate/", "data.json");
	//FilePath = "../web360Info/" + fileName + "/area/";
#endif

	/*
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (Area360LevelName != "")
	{
		if (!PlatformFile.DirectoryExists(*(FilePath + Area360LevelName)))
		{
			PlatformFile.CreateDirectory(*(FilePath+Area360LevelName));
			bResult = true;
		}
	}
	if (Area3DLevelName != "")
	{
		if (!PlatformFile.DirectoryExists(*(FilePath + Area3DLevelName)))
		{
			PlatformFile.CreateDirectory(*(FilePath + Area3DLevelName));
			bResult = true;
		}
	}
	*/
	return bResult;
}

bool UOlimWebConvertFunction::Area3DArrInfoGen(const FString& fileName, const FString& levelName, const FString minimapName, const TArray<ATargetPoint*> targetGath, const FVector2D& scaleCorr, const FVector2D& posCorr)
{
	bool bResult = false;

	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	TSharedPtr<FJsonObject> jsonObj;

	float posCorrX, posCorrY;
	FString outputString;

	jsonValArr.Empty();

	for (int i = 0; i < targetGath.Num(); i++)
	{
		jsonObj = MakeShareable(new FJsonObject);
		if (targetGath[i] != NULL)
		{
			posCorrX = ((targetGath[i]->GetActorLocation().X / scaleCorr.X) - (posCorr.X / scaleCorr.X) + 43 + 160)*0.9657;
			posCorrY = (targetGath[i]->GetActorLocation().Y / scaleCorr.Y) - (posCorr.Y / scaleCorr.Y) + 41 + 153;

			FString TargetName, TargetNameIns;
			targetGath[i]->GetName().Split("_", &TargetName, &TargetNameIns, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			FString TargetNum = FString::FromInt(i);
			if (i < 10) TargetNum = "0" + TargetNum;

			jsonObj->SetStringField("name", fileName + "__" + levelName + "__360Point_" + TargetNum + ".jpg");
			jsonObj->SetNumberField("x", posCorrX);
			jsonObj->SetNumberField("y", posCorrY);
			jsonObj->SetNumberField("floor", 0);
			jsonObj->SetNumberField("dir", DegreeToRad(targetGath[i]->GetActorRotation().Yaw - 90));

			TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
			jsonValArr.Add(jsonValue);
		}
	}
	TSharedPtr<FJsonObject> jsonObjArr = MakeShareable(new FJsonObject);
	jsonObjArr->SetStringField("minimapName", minimapName + ".png");
	jsonObjArr->SetArrayField("pointInfo", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObjArr.ToSharedRef(), jsonWriter);

#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/estate/" + levelName + "/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/estate/" + levelName + "/", "data.json");
#endif

	//result = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "Development/03_ETC", levelName + ".json");

	return bResult;
}


bool UOlimWebConvertFunction::Area360ArrInfoGen(const FString& fileName, const FString& levelName, const FString& minimapName, const TArray<FString>& imageName360, const TArray<float>& direction, const TArray<float>& angle,
	const TArray<FVector2D>& minimapPos, const TArray<FString>& tag, const TArray<int>& selfNum, const TArray<int>& TargetNum, const TArray<FVector>& targetPos)
{
	bool bResult = false;

	TSharedPtr<FJsonObject> jsonObjArr = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	jsonValArr.Empty();
	FString outputString;

	for (int i = 0; i < imageName360.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject);
		jsonObj->SetStringField("name", imageName360[i] + ".jpg");
		jsonObj->SetNumberField("x", (minimapPos[i].X + 160)*0.9657);
		jsonObj->SetNumberField("y", minimapPos[i].Y + 153);
		jsonObj->SetNumberField("dir", DegreeToRad(direction[i] - 90));
		jsonObj->SetNumberField("compass", DegreeToRad(angle[i] - 90));

		TArray<TSharedPtr<FJsonValue>> jsonValArr2;
		jsonValArr2.Empty();

		for (int u = 0; u < selfNum.Num(); u++)
		{
			if (i == selfNum[u])
			{
				TSharedPtr<FJsonObject> jsonObj2 = MakeShareable(new FJsonObject);

				FVector2D Pos = PosToRad(FVector(0, 0, 0), targetPos[u]);
				jsonObj2->SetNumberField("pitch", -Pos.X);
				jsonObj2->SetNumberField("yaw", Pos.Y + 1.57077);
				jsonObj2->SetNumberField("target", TargetNum[u]);
				jsonObj2->SetStringField("tag", tag[u]);

				TSharedRef<FJsonValueObject> jsonValue2 = MakeShareable(new FJsonValueObject(jsonObj2));
				jsonValArr2.Add(jsonValue2);
			}
		}
		if (jsonValArr2.Num() > 0) jsonObj->SetArrayField("linkHotspots", jsonValArr2);

		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
		jsonValArr.Add(jsonValue);
	}

	FString insMinimapName = minimapName;
	if (insMinimapName == "") insMinimapName = "";
	else insMinimapName = insMinimapName + ".png";

	jsonObjArr->SetStringField("minimapName", insMinimapName);
	jsonObjArr->SetArrayField("pointInfo", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObjArr.ToSharedRef(), jsonWriter);
	/*
	jsonObj->SetStringField("name", imageName360 + ".jpg");
	jsonObj->SetNumberField("dir", DegreeToRad(direction - 90));
	jsonObj->SetNumberField("compass", DegreeToRad(angle - 90));

	TArray<TSharedPtr<FJsonValue>> jsonValArr;
	jsonValArr.Empty();

	TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObj));
	jsonValArr.Add(jsonValue);

	TSharedPtr<FJsonObject> jsonObjArr = MakeShareable(new FJsonObject);
	jsonObjArr->SetArrayField("pointInfo", jsonValArr);

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&outputString);
	FJsonSerializer::Serialize(jsonObjArr.ToSharedRef(), jsonWriter);
	*/
#if UE_BUILD_SHIPPING
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../../web360Info/estate/" + levelName + "/", "data.json");
#else
	bResult = UOlimBlueprintFunctionLibrary::SaveStringToFile(outputString, "../web360Info/estate/" + levelName + "/", "data.json");
#endif

	return bResult;
}

FVector2D UOlimWebConvertFunction::PosToRad(const FVector StartPos, const FVector EndPos)
{
	FVector2D RadPitYaw;
	FRotator LookRotation;
	LookRotation = UKismetMathLibrary::FindLookAtRotation(StartPos, EndPos);
	RadPitYaw.X = DegreeToRad(LookRotation.Pitch);
	RadPitYaw.Y = DegreeToRad(LookRotation.Yaw);
	return RadPitYaw;
}
float UOlimWebConvertFunction::DegreeToRad(const float degree)
{
	float rad = degree;
	if (rad > 180) rad = rad - 360;
	else if (rad < -180) rad = rad + 360;

	rad = rad * 0.017453;
	return rad;
}