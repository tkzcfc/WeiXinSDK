// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreUObject.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeiXinSDKFunctions.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class WEIXINSDK_API UWeiXinSDKFunctions : public UObject
{
	GENERATED_BODY()
	
public:

#if PLATFORM_ANDROID 
	static void InitJavaFunctions();
#endif

#if PLATFORM_IOS
	static void InitIOSFunctions();
#endif

	//
	/**
	* 微信是否安装或是否支持API
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static bool WeiXinSDK_IsWXAppInstalledAndSupported();
	
	/**
	* 分享文字到微信
	*
	* @param text              文字内容
	* @param isShareToTimeline false表示分享给朋友，true表示分享到朋友圈
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareText(const FString& text, bool isShareToTimeline);

	/**
	* 发送图片到微信
	* @param title               标题
	* @param description         描述
	* @param imagePath           图片地址
	* @param thumbPath           缩略图图地址
	* @param isShareToTimeline false表示分享给朋友，true表示分享到朋友圈
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareImg(const FString& title, const FString& description, const FString& imagePath, const FString& thumbPath, bool isShareToTimeline);

	/**
	* 分享网址到微信
	*
	* @param webUrl            要分享的网址
	* @param title             标题
	* @param imagePath         图片地址
	* @param description       描述
	* @param isShareToTimeline false表示分享给朋友，true表示分享到朋友圈
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareWeb(const FString& url, const FString& title, const FString& imagePath, const FString& description, bool isShareToTimeline);

};
