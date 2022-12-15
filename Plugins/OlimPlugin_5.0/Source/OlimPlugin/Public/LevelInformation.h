// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OLIMPLUGIN_API LevelInformation
{

private:
	FString ZipViewID;
	FName HomeLevel;
	FName Area3DLevel;
	FName Area360Level;
	FName ExteriorLevel;
	TArray<FName> InteriorLevels;
	TArray<FName> AllLevels;

public:
	LevelInformation(const FString & zipviewID);
	~LevelInformation();

	void GetCurrentLevelInfo(); // ProductionStateNum과 비슷한 기능하기
	TArray<FName> SetAllLevelsFromMapFolder();

	bool IsExteriorExist = false;
	bool IsAreaExist = false;

private:
	bool ReadMapNamesFromMapFolder(const FString& MapFolderPath, TArray<FName>& MapNameArrays);
};
