// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EngineMinimal.h"
#include "Engine.h"

#include "OlimBlueprintFunctionLibrary.generated.h"

#define MIN_SCREEN_WIDTH 1024
#define MIN_SCREEN_HEIGHT 720

/**
 * 
 */
UCLASS()
class OLIMPLUGIN_API UOlimBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static bool CreateNewDirectory(const FString& customDirectoryPath);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString ConvertRelativePathToAbsolutePath(const FString& PathFromContentFolder);
	
	//Text 파일에서 String 읽어들이는 함수! - 나중에 JSON에도 이걸 적용하여 쓰도록 하자. 
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static bool SaveStringToFile(const FString& StringToSave, const FString& PathFromContentFolder, const FString& FileName);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString LoadStringFromFile(const FString& PathFromContentFolder, const FString& FileName);
	
	// Set Texture Setting Property
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static UTexture2D* SetTextureSettingValue(UTexture2D* inSource, TEnumAsByte<TextureCompressionSettings> compSet, TEnumAsByte<TextureGroup> groupSet, const bool gammaSet);
	
	// Make and Save Texture data from SceneCapture Camera of Rendertarget Texture
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static bool MakeFileAndSaveImageData(UTextureRenderTarget2D* inSource, FString FileSavePath, const FString FileNameTag, const FString CurrentLevelName, const int32 Resolution_x, const int32 Resolution_y, const bool HDR);
	
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static bool TakeScreenShot(bool bShowUI, bool DocumentFolder);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static int ReturnNumberByPreNext(int inputNum, const int Length, const bool Next);
	
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static void SetNumberByPreNext(UPARAM(ref) int& inputNum, const int Length, const bool Next);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString GetZipviewID();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static bool ReadFileNamesFromFolder(const FString& FolderPath, const FString& FileExtension, const FString& Contain, const FString& except, TArray<FName>& FileNameArrays);

	//Video Settings 

public: 

	// Get a list of screen resolutions supported on this machine
	//UFUNCTION(BlueprintCallable, Category = "Video Settings")
	//static bool GetSupportedScreenResolutions(TArray<FString>& Resolutions);

	// Get currently set screen resolution
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static FVector2D GetCurrentWindowResolution();

	// Check whether or not we are currently running in fullscreen mode
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool IsInFullscreen();

	// Set the desired screen resolution (does not change it yet)
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool SetScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen);

	// Change the current screen resolution
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool ChangeScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen);

	// Get the current video quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess, int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance);

	// Set the quality settings (not applied nor saved yet)
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool SetVideoQualitySettings(const int32 AntiAliasing = 3, const int32 Effects = 3, const int32 PostProcess = 3,
			const int32 Resolution = 100, const int32 Shadow = 3, const int32 Texture = 3, const int32 ViewDistance = 3);

	// Check whether or not we have vertical sync enabled
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool IsVSyncEnabled();

	// Set the vertical sync flag
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool SetVSyncEnabled(const bool VSync);

	// Confirm and save current video mode (resolution and fullscreen/windowed) as well as quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool SaveVideoModeAndQuality();

	// Revert to original video settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	static bool RevertVideoMode();

private:
	// Try to get the GameUserSettings object from the engine
	static UGameUserSettings* GetGameUserSettings();
		
	
};
