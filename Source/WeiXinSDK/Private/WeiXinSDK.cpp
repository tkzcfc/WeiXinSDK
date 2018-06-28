// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "WeiXinSDK.h"
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"
//#include "ExampleLibrary.h"
//#include "ISettingsModule.h"
#include "WeiXinSDKFunctions.h"


#define LOCTEXT_NAMESPACE "FWeiXinSDKModule"

class FWeiXinSDKModule : public IWeiXinSDKModule
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FWeiXinSDKModule, WeiXinSDK)

void FWeiXinSDKModule::StartupModule()
{
#if PLATFORM_ANDROID 
	UWeiXinSDKFunctions::InitAndroidFunctions();
#endif
}

void FWeiXinSDKModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
