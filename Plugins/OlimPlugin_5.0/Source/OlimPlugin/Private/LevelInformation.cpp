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

	//Exterior�ʰ� Area360���� ���������� ó���ϱ� ���� �ڿ������� iteration�Ͽ� �����. 
	for (int32 index = 0; index < MapFiles.Num(); index++)
	{
		// exterior�� �ƴϸ�
		if (!(MapFiles[index].Contains(ZipViewID + "_Exterior"))) ///check
		{
			// Ȯ���� ����
			FString strCleanFileName = MapFiles[index].Replace(TEXT(".umap"), TEXT(""));
			
			// MapName�� �߰�
			MapNameArrays.Add(FName(*strCleanFileName));
			UE_LOG(LogTemp, Log, TEXT("MapNameArrays added! : %s "), *MapFiles[index]);
		}

		else
		{
			// Exterior ���� ����
			IsExteriorExist = true;
			ExteriorLevel = FName(MapFiles[index].Replace(TEXT(".umap"), TEXT("")));
		}

		// Area�̸�
		if (MapFiles[index].Contains(ZipViewID + "_Area"))
		{
			// Area ���� ����
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