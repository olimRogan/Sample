// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimBlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"

FString UOlimBlueprintFunctionLibrary::ConvertRelativePathToAbsolutePath(const FString& PathFromContentFolder)
{
	FString AbsolutePath;	
	FString RootFolderPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	FString FolderPath = PathFromContentFolder;
	
	if (FolderPath.Contains(TEXT("../")))
	{
		//만약 찾으려는 폴더 경로가 상위를 향한다면, Content폴더의 원래 절대 경로에서 현재 폴더를 삭제해야 한다.
		while (FolderPath.Find(TEXT("../")) == 0)
		{
			FolderPath.RemoveFromStart(TEXT("../"));

			//마지막 캐릭터가 '/'로 끝날 테니 그 캐릭터를 일단 자른다.		
			FString strTemp = RootFolderPath.LeftChop(1);
			int32 LastCharIndex = 0;
			strTemp.FindLastChar('/', LastCharIndex);
			int32 count = strTemp.Len() - (LastCharIndex + 1);
			RootFolderPath = strTemp.LeftChop(count);

			//UE_LOG(LogTemp, Log, TEXT("Choped FolderPath is : %s "), *RootFolderPath);
		}

		if (!FolderPath.IsEmpty())
		{
			if (FolderPath.EndsWith("/"))
			{
				AbsolutePath = RootFolderPath + FolderPath;
			}
			else
			{
				AbsolutePath = RootFolderPath + FolderPath + "/";
			}
		}
		else
		{
			AbsolutePath = RootFolderPath;
		}
	}
	else
	{
		if (FolderPath.EndsWith("/"))
		{
			AbsolutePath = RootFolderPath + FolderPath;
		}
		else
		{
			AbsolutePath = RootFolderPath + FolderPath + "/";
		}		
	}
	return AbsolutePath;
}


bool UOlimBlueprintFunctionLibrary::SaveStringToFile(const FString& StringToSave, const FString& PathFromContentFolder, const FString& FileName)
{

	FString FullPathFromRoot;
	FullPathFromRoot = ConvertRelativePathToAbsolutePath(PathFromContentFolder) + FileName;

	UE_LOG(LogTemp, Log, TEXT("Text File saved at %s via OlimBlueprintFunctionLibrary!"), *FullPathFromRoot);

	return FFileHelper::SaveStringToFile(StringToSave, *FullPathFromRoot, FFileHelper::EEncodingOptions::ForceUTF8);
}


FString UOlimBlueprintFunctionLibrary::LoadStringFromFile(const FString& PathFromContentFolder, const FString& FileName)
{	
	FString FullPathFromRoot;
	FullPathFromRoot = ConvertRelativePathToAbsolutePath(PathFromContentFolder) + FileName;

	bool bLoad = false;
	FString StringToLoad;	
	bLoad = FFileHelper::LoadFileToString(StringToLoad, *FullPathFromRoot);

	if (bLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("Text File at %s is loaded via OlimBlueprintFunctionLibrary!") , *FullPathFromRoot);
		return StringToLoad;	
	}
		
	UE_LOG(LogTemp, Log, TEXT("LoadStringFromFile has been failed..."));
	return FString("");
}


// Set Texture comppression and group Property
UTexture2D* UOlimBlueprintFunctionLibrary::SetTextureSettingValue(UTexture2D* inSource, TEnumAsByte<TextureCompressionSettings> compSet, 
	TEnumAsByte<TextureGroup> groupSet, const bool gammaSet)
{
	inSource->CompressionSettings = compSet;
	inSource->LODGroup = groupSet;
	inSource->SRGB = gammaSet;
	return inSource;
}



//작업진행중
// Make and Save Texture data from SceneCapture Camera of Rendertarget Texture
bool UOlimBlueprintFunctionLibrary::MakeFileAndSaveImageData(UTextureRenderTarget2D* inSource, FString FileSavePath, 
	const FString FileNameTag, const FString CurrentLevelName, const int32 Resolution_x, const int32 Resolution_y, const bool HDR)
{
	/*
	UTexture2D* Result = nullptr;
	FRenderTarget* TargetResource = inSource->GameThread_GetRenderTargetResource();
	if (TargetResource == nullptr)
	{
		return false;
	}
	TArray<FColor> RawPixels;
	if (!TargetResource->ReadPixels(RawPixels))
	{
		return false;
	}
	else
	{
		for (FColor& Pixel : RawPixels)
		{
		const uint8 R = Pixel.B;
		const uint8 B = Pixel.R;
		Pixel.R = R;
		Pixel.B = B;
		Pixel.A = 255;
		}
	}
	
	FString TextureFileName;
	TextureFileName = FPaths::Combine(FileNameTag, CurrentLevelName);
	FString SavedPath = FPaths::Combine(FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()), FileSavePath);

	// BMP
	FTextureRenderTargetResource* RenderTargetResource = inSource->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(HDR); // 감마를 적용할 것인지 -> 제작팀 확인 필요

	TArray<FColor> OutBMP;
	OutBMP.InsertZeroed(0, Resolution_x*Resolution_y);
	RenderTargetResource->ReadPixels(OutBMP, ReadPixelFlags);

	for (FColor& color : OutBMP)
	{
		color.A = 0;
	}

	FIntPoint DestSize(Resolution_x, Resolution_y);
	FHighResScreenshotConfig& ResConfig = GetHighResScreenshotConfig();
	bool isSaved = ResConfig.SaveImage(TextureFileName, OutBMP, DestSize, &SavedPath);

	TArray<uint8> CompressedBitmap;
	FImageUtils::CompressImageArray(DestSize.X, DestSize.Y, OutBMP, CompressedBitmap);
	bool isFiled = FFileHelper::SaveArrayToFile(CompressedBitmap, *SavedPath);
	*/
	return true;
}


bool UOlimBlueprintFunctionLibrary::TakeScreenShot(bool bShowUI, bool DocumentFolder)
{
	bool bResult = false;
			
	FString FullPathFromRoot;
	FString SaveFolderPath;
	FString ProjectName;
	FString ScreenShotFileName;

	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectName"), ProjectName, GGameIni);

#if UE_BUILD_SHIPPING
	SaveFolderPath = "../../../ContentSet/" + ProjectName + "/";		
#else
	SaveFolderPath = "../../ContentSet/" + ProjectName + "/";
#endif

	ScreenShotFileName = "ScreenShot_" + FDateTime::Now().ToString() + ".png";
	FullPathFromRoot = ConvertRelativePathToAbsolutePath(SaveFolderPath) + ScreenShotFileName;

	if (DocumentFolder == true)
	{
		SaveFolderPath = FPlatformProcess::UserDir();
		FullPathFromRoot = SaveFolderPath + "/ZipviewScreeshot/" + ProjectName + "/" + ScreenShotFileName;
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Screen Shot has been taken!"));
	FScreenshotRequest::RequestScreenshot(FullPathFromRoot, bShowUI, false);
	bResult = FScreenshotRequest::IsScreenshotRequested();

	return bResult;
}


int UOlimBlueprintFunctionLibrary::ReturnNumberByPreNext(int inputNum, const int Length, const bool Next)
{
	int CalNum;
	CalNum = inputNum;
	if (Next)
	{
		CalNum++;
		if (CalNum >= Length) CalNum = 0;
	}
	else
	{
		CalNum--;
		if (CalNum < 0) CalNum = Length -1;
	}
	return CalNum;
}

void UOlimBlueprintFunctionLibrary::SetNumberByPreNext(UPARAM(ref) int& inputNum, const int Length, const bool Next)
{
	int calNum;
	calNum = inputNum;
	if (Next)
	{
		calNum++;
		if (calNum >= Length) calNum = 0;
	}
	else
	{
		calNum--;
		if (calNum < 0) calNum = Length - 1;
	}
	inputNum = calNum;	
}


FString UOlimBlueprintFunctionLibrary::GetZipviewID()
{
	FString ZipViewID, InsParam;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectName"), InsParam, GGameIni);
	InsParam.Split("_", &ZipViewID, &InsParam, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	return ZipViewID;
}

bool UOlimBlueprintFunctionLibrary::ReadFileNamesFromFolder(const FString& FolderPath, const FString& FileExtension, 
	const FString& Contain, const FString& except, TArray<FName>& FileNameArrays)
{
	bool bResult = false;
	FString FolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + FolderPath;

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> Files;
	Files.Empty();
	FileNameArrays.Empty();

	FileManager.FindFiles(Files, *FolderFullPath, *FileExtension);

	for (int32 index = 0; index < Files.Num(); index++)
	{
		if (Files[index].Contains(Contain))
		{
			if (Files[index].Contains(except))
			{

			}
			else
			{
				FileNameArrays.Add(FName(*Files[index]));
				bResult = true;
			}
		}
	}
	return bResult;
}

/*
// Get a list of screen resolutions supported by video adapter
// NOTE: This function needs "RHI". add to <Project>.Build.cs file!
bool UOlimBlueprintFunctionLibrary::GetSupportedScreenResolutions(TArray<FString>& Resolutions)
{
	FScreenResolutionArray ResolutionsArray;

	if (RHIGetAvailableResolutions(ResolutionsArray, true))  // needs the "RHI" dependency
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			if (Resolution.Width < MIN_SCREEN_WIDTH || Resolution.Height < MIN_SCREEN_HEIGHT)
			{
				continue;
			}

			FString Str = FString::FromInt(Resolution.Width) + "x" + FString::FromInt(Resolution.Height);
			Resolutions.AddUnique(Str);
			//UE_LOG(LogTemp, Log, TEXT("%s"), *Str);
		}
		return true;
	}
	return false;  // failed to obtain screen resolutions
}
*/


// Get currently set screen resolution
FVector2D UOlimBlueprintFunctionLibrary::GetCurrentWindowResolution()
{	
	/*
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return FString("");
	}
	FIntPoint Resolution = Settings->GetScreenResolution();
	return FString::FromInt(Resolution.X) + "x" + FString::FromInt(Resolution.Y);
	*/
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	return ViewportSize;
}


// Check whether or not we are currently running in fullscreen mode
bool UOlimBlueprintFunctionLibrary::IsInFullscreen()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	return Settings->GetFullscreenMode() == EWindowMode::Fullscreen;
}


// Set the desired screen resolution (does not change it yet)
bool UOlimBlueprintFunctionLibrary::SetScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	Settings->SetScreenResolution(FIntPoint(Width, Height));
	Settings->SetFullscreenMode(Fullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
	return true;
}


// Change the current screen resolution
bool UOlimBlueprintFunctionLibrary::ChangeScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	EWindowMode::Type WindowMode = Fullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed;
	Settings->RequestResolutionChange(Width, Height, WindowMode, false);
	return true;
}


// Get the current video quality settings
bool UOlimBlueprintFunctionLibrary::GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess,
	int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	Effects = Settings->ScalabilityQuality.EffectsQuality;
	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	Texture = Settings->ScalabilityQuality.TextureQuality;
	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;
	return true;
}


// Set the quality settings (not applied nor saved yet)
bool UOlimBlueprintFunctionLibrary::SetVideoQualitySettings(const int32 AntiAliasing, const int32 Effects, const int32 PostProcess,
	const int32 Resolution, const int32 Shadow, const int32 Texture, const int32 ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	Settings->ScalabilityQuality.EffectsQuality = Effects;
	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	Settings->ScalabilityQuality.TextureQuality = Texture;
	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	return true;
}


// Check whether or not we have vertical sync enabled
bool UOlimBlueprintFunctionLibrary::IsVSyncEnabled()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	return Settings->IsVSyncEnabled();
}


// Set the vertical sync flag
bool UOlimBlueprintFunctionLibrary::SetVSyncEnabled(const bool VSync)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	Settings->SetVSyncEnabled(VSync);
	return true;
}


// Confirm and save current video mode (resolution and fullscreen/windowed)
bool UOlimBlueprintFunctionLibrary::SaveVideoModeAndQuality()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	Settings->ConfirmVideoMode();
	Settings->ApplyNonResolutionSettings();
	Settings->SaveSettings();
	return true;
}


// Revert to original video settings
bool UOlimBlueprintFunctionLibrary::RevertVideoMode()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}
	Settings->RevertVideoMode();
	return true;
}

bool UOlimBlueprintFunctionLibrary::CreateNewDirectory(const FString& customDirectoryPath)
{
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	bool createDirSuccess = false;

	if (!FileManager.DirectoryExists(*customDirectoryPath))
	{
		createDirSuccess = FileManager.CreateDirectoryTree(*customDirectoryPath);
	}

	return createDirSuccess;
}

//---- PRIVATE METHODS -------------------------------------------------------------------------------

// Try to get the GameUserSettings object from the engine
UGameUserSettings* UOlimBlueprintFunctionLibrary::GetGameUserSettings()
{
	if (GEngine != nullptr)
	{
		return GEngine->GameUserSettings;
	}
	return nullptr;
}

