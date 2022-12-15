// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelInformation.h"

LevelInformation::LevelInformation(const FString& zipviewID)
{
	ZipViewID = zipviewID;
}

LevelInformation::~LevelInformation()
{
}

void LevelInformation::GetCurrentLevelInfo()
{

}

TArray<FName> LevelInformation::SetAllLevelsFromMapFolder()
{
	FString mapFolderPath = "Unique/" + ZipViewID + "/3DResources/Maps";

	ReadMapNamesFromMapFolder(mapFolderPath, AllLevels);

	return AllLevels;
}

bool LevelInformation::ReadMapNamesFromMapFolder(const FString& MapFolderPath, TArray<FName>& MapNameArrays)
{
	bool bResult = false;

	FString MapFolderFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + MapFolderPath;

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> MapFiles;
	MapFiles.Empty();
	MapNameArrays.Empty();

	FileManager.FindFiles(MapFiles, *MapFolderFullPath, TEXT("umap"));
	UE_LOG(LogTemp, Log, TEXT("Map Files Num : %d "), MapFiles.Num());

	//Exterior맵과 Area360맵은 예외적으로 처리하기 위해 뒤에서부터 iteration하여 지운다. 
	for (int32 index = 0; index < MapFiles.Num(); index++)
	{
		// exterior가 아니면
		if (!(MapFiles[index].Contains(ZipViewID + "_Exterior"))) ///check
		{
			// 확장자 뗀다
			FString strCleanFileName = MapFiles[index].Replace(TEXT(".umap"), TEXT(""));
			
			// MapName에 추가
			MapNameArrays.Add(FName(*strCleanFileName));
			UE_LOG(LogTemp, Log, TEXT("MapNameArrays added! : %s "), *MapFiles[index]);
		}

		else
		{
			// Exterior 맵이 있음
			IsExteriorExist = true;
			ExteriorLevel = FName(MapFiles[index].Replace(TEXT(".umap"), TEXT("")));
		}

		// Area이면
		if (MapFiles[index].Contains(ZipViewID + "_Area"))
		{
			// Area 맵이 있음
			IsAreaExist = true;
			if (MapFiles[index].Contains("360"))
			{
				Area360Level = FName(MapFiles[index].Replace(TEXT(".umap"), TEXT("")));
			}
			else
			{
				Area3DLevel = FName(MapFiles[index].Replace(TEXT(".umap"), TEXT("")));
			}
		}
	}
	bResult = (MapNameArrays.Num() != 0) ? true : false;

	return bResult;
}