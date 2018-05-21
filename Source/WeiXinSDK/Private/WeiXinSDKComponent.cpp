// Fill out your copyright notice in the Description page of Project Settings.

#include "WeiXinSDKComponent.h"


void UWeiXinSDKComponent::OnRegister()
{
	Super::OnRegister();

	UWeiXinSDKComponent::onWeiXinShareResultDelegate.AddUObject(this, &UWeiXinSDKComponent::onWeiXinShareResult_Handler);
}

void UWeiXinSDKComponent::OnUnregister()
{
	Super::OnUnregister();

	UWeiXinSDKComponent::onWeiXinShareResultDelegate.RemoveAll(this);
}

UWeiXinSDKComponent::FWeiXinSDKonWeiXinShareResultDelegate UWeiXinSDKComponent::onWeiXinShareResultDelegate;