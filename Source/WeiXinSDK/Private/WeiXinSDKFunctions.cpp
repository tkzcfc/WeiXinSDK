// Fill out your copyright notice in the Description page of Project Settings.

#include "WeiXinSDKFunctions.h"
#include "WeiXinSDKComponent.h"

#if PLATFORM_ANDROID

#include "Android/AndroidJNI.h"
#include "AndroidApplication.h"

#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
	name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
	check(name != NULL); \
} else { \
	check(0); \
}

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqText);
DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqImg);
DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqWeb);

void UWeiXinSDKFunctions::InitJavaFunctions()
{
	//(String text, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqText, "(Ljava/lang/String;B;)V");
	//(int index, String url, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqImg, "(I;Ljava/lang/String;B;)V");
	//(String webUrl, String title, String imagePath, String description, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqWeb, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;B;)V");
}

#undef DECLARE_JAVA_METHOD
#undef INIT_JAVA_METHOD

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnWXShareResult(JNIEnv* jenv, jobject thiz, jstring result)
{
	FString Result;

	const char* charsResult = jenv->GetStringUTFChars(result, 0);
	Result = FString(UTF8_TO_TCHAR(charsResult));
	jenv->ReleaseStringUTFChars(result, charsResult);

	

	FGraphEventRef EnterBackgroundTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UWeiXinSDKComponent::onWeiXinShareResultDelegate.Broadcast(Result);
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

#endif


void UWeiXinSDKFunctions::WeiXinSDK_ShareText(const FString& text, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring TextArg = Env->NewStringUTF(TCHAR_TO_UTF8(*text));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_sendReqText, TextArg, (jboolean)isShareToTimeline);

		Env->DeleteLocalRef(TextArg);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_sendReqText()\n"));
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif
	FGraphEventRef EnterBackgroundTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UWeiXinSDKComponent::onWeiXinShareResultDelegate.Broadcast(TEXT("66666666"));
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void UWeiXinSDKFunctions::WeiXinSDK_ShareImg(int index, const FString& url, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring URLArg = Env->NewStringUTF(TCHAR_TO_UTF8(*url));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_sendReqImg, (jint)index, URLArg, (jboolean)isShareToTimeline);

		Env->DeleteLocalRef(URLArg);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_sendReqImg()\n"));
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif
}

//(String webUrl, String title, String description, boolean isShareToTimeline)
void UWeiXinSDKFunctions::WeiXinSDK_ShareWeb(const FString& url, const FString& title, const FString& imagePath, const FString& description, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring URLArg = Env->NewStringUTF(TCHAR_TO_UTF8(*url));
		jstring TitleArg = Env->NewStringUTF(TCHAR_TO_UTF8(*title));
		jstring ImagePathArg = Env->NewStringUTF(TCHAR_TO_UTF8(*imagePath));
		jstring DescriptionArg = Env->NewStringUTF(TCHAR_TO_UTF8(*description));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_sendReqWeb, URLArg, TitleArg, ImagePathArg, DescriptionArg, (jboolean)isShareToTimeline);

		Env->DeleteLocalRef(URLArg);
		Env->DeleteLocalRef(TitleArg);
		Env->DeleteLocalRef(ImagePathArg);
		Env->DeleteLocalRef(DescriptionArg);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_sendReqWeb()\n"));
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif
}
