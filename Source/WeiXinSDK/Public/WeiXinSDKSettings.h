// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ScriptMacros.h"
#include "WeiXinSDKSettings.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class WEIXINSDK_API UWeiXinSDKSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UWeiXinSDKSettings(const FObjectInitializer& ObjectInitializer);
};
