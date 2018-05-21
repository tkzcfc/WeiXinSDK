// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeiXinSDKFunctions.h"
#include "WeiXinSDKComponent.generated.h"


UCLASS(ClassGroup = Advertising, HideCategories = (Activation, "Components|Activation", Collision), meta = (BlueprintSpawnableComponent))
class WEIXINSDK_API UWeiXinSDKComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FWeiXinSDKonWeiXinShareResultDelegate, FString);

	static FWeiXinSDKonWeiXinShareResultDelegate onWeiXinShareResultDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeiXinSDKonWeiXinShareResultDynDelegate, FString, Error);

	UPROPERTY(BlueprintAssignable)
	FWeiXinSDKonWeiXinShareResultDynDelegate onWeiXinShareResult;


	void OnRegister() override;
	void OnUnregister() override;

private:
	UFUNCTION()
	void onWeiXinShareResult_Handler(FString Error) { onWeiXinShareResult.Broadcast(Error); }
	
};
