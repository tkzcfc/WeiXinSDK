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
	
	/**
	* �������ֵ�΢��
	*
	* @param text              ��������
	* @param isShareToTimeline false��ʾ���������ѣ�true��ʾ����������Ȧ
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareText(const FString& text, bool isShareToTimeline);

	/**
	* ����ͼƬ��΢��
	*
	* @param index         ������1����ʾ�ֻ�����ͼƬ��2����ʾ����ͼƬ(ͼƬ��URL)
	* @param url           ͼƬ��ַ
	* @param isShareToTimeline false��ʾ���������ѣ�true��ʾ����������Ȧ
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareImg(int index, const FString& url, bool isShareToTimeline);

	/**
	* ������ַ��΢��
	*
	* @param webUrl            Ҫ��������ַ
	* @param title             ����
	* @param description       ����
	* @param isShareToTimeline false��ʾ���������ѣ�true��ʾ����������Ȧ
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "WeiXinSDK "), Category = "WeiXinSDK|")
	static void WeiXinSDK_ShareWeb(const FString& url, const FString& title, const FString& imagePath, const FString& description, bool isShareToTimeline);

};