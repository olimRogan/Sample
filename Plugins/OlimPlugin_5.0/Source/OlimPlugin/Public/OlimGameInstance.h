// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Texture2D.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "InteriorInformations.h"
#include "LevelInformation.h"
#include "OverrideData.h"
#include "OlimGameInstance.generated.h"


/**
 * 
 */



UENUM(BlueprintType)
enum class UEZipViewMode : uint8
{
	ZV_HOME_MODE		  UMETA(DisplayName = "HomeMode"),
	ZV_EXTERIOR_MODE	  UMETA(DisplayName = "ExteriorMode"),
	ZV_INTERIOR_MODE	  UMETA(DisplayName = "InteriorMode"),
	ZV_SELECT_MODE		  UMETA(DisplayName = "SelectTypeMode"),
	ZV_INFO_MODE		  UMETA(DisplayName = "InfoMode"),
	ZV_VR_MODE			  UMETA(DisplayName = "VRMode"),
	ZV_MAP_MODE	          UMETA(DisplayName = "MapMode"),
	ZV_AREA_SELECT_MODE	  UMETA(DisplayName = "SelectAreaMode"),
	ZV_AREA_MODE	      UMETA(DisplayName = "AreaMode"),
	ZV_CONDITION_MODE	  UMETA(DisplayName = "ConditionMode"),
};

//USTRUCT(BlueprintType)
//struct FProductionState
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") FString MapName;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int MapAllNum;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int MapReleaseNum;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") bool isReleaseMap;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgNormalNum;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgNormalPtr;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgHoverNum;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgHoverPtr;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") int ImgInfoNum;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic") UTexture2D* ImgInfoPtr;
//
//	/*
//	UPROPERTY(BlueprintReadOnly) int ImgMinimapNum;
//	UPROPERTY(BlueprintReadOnly) UTexture2D* ImgMinimapPtr;
//	UPROPERTY(BlueprintReadOnly) int ImgMinimapSizeNum;
//	UPROPERTY(BlueprintReadOnly) UTexture2D* ImgMinimapSizePtr;
//	UPROPERTY(BlueprintReadOnly) int ImgMinimapSize_t_Num;
//	UPROPERTY(BlueprintReadOnly) UTexture2D* ImgMinimapSize_t_Ptr;
//	*/
//};


UCLASS()
class OLIMPLUGIN_API UOlimGameInstance : public UGameInstance
{
	GENERATED_BODY()	

public:
	UOlimGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic")
	FString ZipViewID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic")
	FString ZipViewToken;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ZipView Basic")
	bool ZipViewCertification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	UEZipViewMode CurrentZipViewMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	int CurrentInteriorMapNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	int CurrentArea360Num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	int CurrentArea3DNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	int AreaPageNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	int32 AllInteriorLevelsNum;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZipView Data")
	TArray<FInteriorSelectImage> arrInteriorSelectImg;

    // ����
	UFUNCTION(BlueprintCallable, Category = "ZipView Data")
	void HttpGet(FString URL, FString Token);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ZipView Data")
	bool isNetworkConnection;


	//���ۻ��¿� ���� ����ü�� Tarray�� ����, �ʱ⿡ �ѹ� �����Ͽ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZipView Data")
	TArray<FProductionState> arrProductionState;

	// ���� ���̸��� �����ͼ� arrProductionState�� �������� ��ȯ
	UFUNCTION(BlueprintCallable, Category = "ZipView Data")
	int ProductionStateNum(const FString& CurrentMap);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Global Variables")
	FString AreaBtnInfo;

	/**************************Json SAVE/LOAD Variables**************************/
	/***UniqueSetting.json***/ // Content/Development/03_ETC ���� �ȿ� ��ġ�Ѵ�. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	bool bLowPerformance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	bool bInfoExtractMode;

	/****** 360 캡처용 임시 변수*******/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "360 Capture")
	bool InfoExtractModeWithCapture;


	//���� ������������ �����ϸ鼭 ���� ���� Json ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	bool bZipviewEnterPrise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	bool bProductionInfoShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	bool bGuideBookPDFShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString strExteriorInfoMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString strInteriorInfoMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString strAreaInfoMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString strMapURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString strConditionURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString MainColorString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FString SubColorString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FColor MainColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Unique")
	FColor SubColor;



	/***CommonSetting.json***/ //��� ������Ʈ�� �����ϴ� ContentSet ���� �ȿ� ��ġ�Ѵ�. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bTotalAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bFullHmdMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bExteriorMenuNotShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bInteriorMenuNotShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bEstateMenuNotShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bMapMenuNotShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bConditionMenuNotShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Json Common")
	bool bGuideBookMenuNotShow;

	/**************************Json SAVE/LOAD Variables**************************/

	
	//�� ���ϵ��� ����ִ� ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString MapFolder;
	
	//�� �̸��� - ���� ����ִ� �������� ���ϸ��� �о� set�� ���̹Ƿ� readonly�� �صд�.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Settings")
	TArray<FName> MapNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool IsExteriorExist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool IsAreaExist;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool ReadMapNamesFromMapFolder(const FString& MapFolderPath, TArray<FName>& MapNameArrays);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	static bool ReadAssetFileNamesFromFolder(const FString& FolderPath, const FString& KeyWord, TArray<FName>& FileNameArrays);

	//Dynamic�ϰ� asset���� Load�ϴ� static�Լ���
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static TArray<UObject*> DynamicLoadObjFromPath(const FString& PathFromContentFolder, UClass* AssetClass = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static TArray<UTexture2D*> DynamicLoadTexture2DFromPath(const FString& PathFromContentFolder);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static TArray<UTexture2D*> DynamicLoadTextureWithKeyword(const FString& PathFromContentFolder, const FString& Keyword = "");

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static TArray<UObject*> DynamicLoadObjectWithKeyword(const FString& PathFromContentFolder, const FString& Keyword = "");

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	TArray<UTexture2D*> DynamicLoadTextureWithMapAndKeyword(const FString& PathFromContentFolder, const FString& prefix = "", const FString& suffix = "");

	//String���� Ư�� Keyword�� ���� ���ϴ� ���ڿ��� ���� (�����̸����� �� �̸����� �����ϱ� ����)
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	FString ExtractionStringByKeyword(FString inSorce, const FString& prefix = "", const FString& suffix = "");


	/**************************Json $ Text SAVE/LOAD Function**************************/		
	UFUNCTION(BlueprintCallable, Category = "Json Function")
	bool SaveCommonGameData();
	
	UFUNCTION(BlueprintCallable, Category = "Json Function")
	bool LoadCommonGameData();	

	UFUNCTION(BlueprintCallable, Category = "Json Function")
	bool SaveUniqueGameData();

	UFUNCTION(BlueprintCallable, Category = "Json Function")
	bool LoadUniqueGameData();

	UFUNCTION(BlueprintCallable, Category = "Json Function")
	FString LoadAreaTextData();

	UFUNCTION(BlueprintCallable, Category = "Json Function")
	FString LoadProjectConfig(const FString &config);


private:
	//���Ӹ�� �ڵ� ���� ����
	void SettingGameMode();
	void BasicSet();

protected:
	virtual void Init();
	virtual void PostInitProperties();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);	
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Etc")
		bool bOverride = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Etc")
		FOverrideDataStructure OverrideData;
};


