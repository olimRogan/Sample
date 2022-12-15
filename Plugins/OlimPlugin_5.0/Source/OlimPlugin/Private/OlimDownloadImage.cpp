// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimDownloadImage.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "Misc/FileHelper.h"


//----------------------------------------------------------------------//
// UAsyncTaskDownloadImage
//----------------------------------------------------------------------//
#if !UE_SERVER

static void WriteRawToTexture_RenderThread(FTexture2DDynamicResource* TextureResource, TArray64<uint8>* RawData, bool bUseSRGB = true)
{
	check(IsInRenderingThread());

	FRHITexture2D* TextureRHI = TextureResource->GetTexture2DRHI();

	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* DestData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

	for (int32 y = 0; y < Height; y++)
	{
		uint8* DestPtr = &DestData[((int64)Height - 1 - y) * DestStride];

		const FColor* SrcPtr = &((FColor*)(RawData->GetData()))[((int64)Height - 1 - y) * Width];
		for (int32 x = 0; x < Width; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	RHIUnlockTexture2D(TextureRHI, 0, false, false);
	delete RawData;
}

#endif
UOlimDownloadImage::UOlimDownloadImage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		AddToRoot();
	}
}

UOlimDownloadImage* UOlimDownloadImage::DownloadImage(FString URL)
{
	UOlimDownloadImage* DownloadTask = NewObject<UOlimDownloadImage>();
	DownloadTask->Start(URL);

	return DownloadTask;
}

void UOlimDownloadImage::Start(FString URL)
{
	Url = URL;
#if !UE_SERVER
	TArray<uint8> Bytes;
	Bytes.SetNumUninitialized(URL.Len());
	StringToBytes(URL, Bytes.GetData(), Bytes.Num());
	FMD5 md5;
	FString Hash = md5.HashBytes(Bytes.GetData(), Bytes.Num());
	FString RelativePath = FPaths::ProjectSavedDir() + "/tmp/" + Hash;
	FString FullPath = FPaths::ConvertRelativePathToFull(RelativePath);

	FString FullPathJpeg = FullPath + ".jpeg";
	FString FullPathPng = FullPath + ".png";
	//if (FPaths::FileExists(FullPathJpeg))
	//{
	//	TArray<uint8> data;
	//	FFileHelper::LoadFileToArray(data, *FullPathJpeg);

	//	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//	HttpRequest->OnHeaderReceived().BindLambda([this, data](FHttpRequestPtr /*Request*/, const FString& /*HeaderName*/, const FString& /*NewHeaderValue*/) {
	//		HandleImageRequest(data);
	//	});
	//	//HttpRequest->OnProcessRequestComplete().BindLambda([this, data](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded) {
	//	//	HandleImageRequest(data);
	//	//});
	//	HttpRequest->SetURL("https://www.google.com/");
	//	HttpRequest->SetVerb(TEXT("GET"));
	//	HttpRequest->ProcessRequest();
	//	
	//	
	//}
	//else if (FPaths::FileExists(FullPathPng))
	//{
	//	TArray<uint8> data;
	//	FFileHelper::LoadFileToArray(data, *FullPathPng);
	//	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//	HttpRequest->OnHeaderReceived().BindLambda([this, data](FHttpRequestPtr /*Request*/, const FString& /*HeaderName*/, const FString& /*NewHeaderValue*/) {
	//		HandleImageRequest(data);
	//	});
	//	//HttpRequest->On().BindLambda([](FHttpRequestPtr /*Request*/, const FString& /*HeaderName*/, const FString& /*NewHeaderValue*/) {});
	//	//HttpRequest->OnProcessRequestComplete().BindLambda([this, data](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded) {
	//	//	HandleImageRequest(data);
	//	//});
	//	HttpRequest->SetURL("https://www.google.com/");
	//	HttpRequest->SetVerb(TEXT("GET"));
	//	HttpRequest->ProcessRequest();
	//}
	//else
	{
		auto Request = FHttpModule::Get().CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &UOlimDownloadImage::HandleImageRequest);
		Request->SetURL(URL);
		Request->SetVerb(TEXT("GET"));
		Request->ProcessRequest();
	}

#else
	// On the server we don't execute fail or success we just don't fire the request.
	RemoveFromRoot();
#endif
}

void UOlimDownloadImage::HandleImageRequest(TArray<uint8> data)
{

#if !UE_SERVER
	//RemoveFromRoot();

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[3] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	};

	UTexture2DDynamic* Texture = nullptr;
	for (auto ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(data.GetData(), data.Num()))
		{
			TArray64<uint8>* RawData = new TArray64<uint8>();
			const ERGBFormat InFormat = ERGBFormat::BGRA;
			if (ImageWrapper->GetRaw(InFormat, 8, *RawData))
			{
				Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
				if (Texture != nullptr)
				{
					Texture->SRGB = true;
					Texture->UpdateResource();

					FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(Texture->GetResource());
					//WriteRawToTexture_RenderThread(TextureResource, RawData);
					ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)([TextureResource, RawData](FRHICommandListImmediate& RHICmdList)
					{
						WriteRawToTexture_RenderThread(TextureResource, RawData);
					});

					break;
				}
			}
		}
	}

	FVector2D size;
	if (Texture != nullptr)
	{
		size.X = Texture->SizeX, size.Y = Texture->SizeY;
		OnSuccess2.Broadcast(Url, Texture, size);
	}
	else
	{
		OnFail2.Broadcast(Url, nullptr, FVector2D(0, 0));
	}

#endif
}

void UOlimDownloadImage::HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	static TMap<FString, FString> ExtensionMimeTypeMap;
	if (ExtensionMimeTypeMap.Num() == 0)
	{
		ExtensionMimeTypeMap.Add(TEXT("image/bmp"), TEXT(".bmp"));
		ExtensionMimeTypeMap.Add(TEXT("image/jpeg"), TEXT(".jpeg"));
		ExtensionMimeTypeMap.Add(TEXT("image/png"), TEXT(".png"));
	}

#if !UE_SERVER
	if (bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0)
	{
		FString URL = HttpRequest->GetURL();
		TArray<uint8> Bytes;
		Bytes.SetNumUninitialized(URL.Len());
		StringToBytes(URL, Bytes.GetData(), Bytes.Num());

		FMD5 md5;
		FString Hash = md5.HashBytes(Bytes.GetData(), Bytes.Num());
		FString RelativePath = FPaths::ProjectSavedDir() + "/tmp/" + Hash;
		if (FString* extension = ExtensionMimeTypeMap.Find(HttpResponse->GetContentType()))
			RelativePath.Append(*extension);
		FString FullPath = FPaths::ConvertRelativePathToFull(RelativePath);
		//FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *FullPath);
		HandleImageRequest(HttpResponse->GetContent());
		return;
	}
	OnFail2.Broadcast(Url, nullptr, FVector2D(0, 0));
#endif
}
