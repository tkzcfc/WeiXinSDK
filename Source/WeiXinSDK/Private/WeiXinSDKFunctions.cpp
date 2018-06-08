// Fill out your copyright notice in the Description page of Project Settings.

#include "WeiXinSDKFunctions.h"
#include "WeiXinSDKComponent.h"


//#include <string>
//#include <locale.h> 
//
//std::string wx_ws2s(const std::wstring& ws)
//{
//	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  
//
//	setlocale(LC_ALL, "chs");
//
//	const wchar_t* _Source = ws.c_str();
//	std::size_t _Dsize = 2 * ws.size() + 1;
//	char *_Dest = new char[_Dsize];
//	memset(_Dest, 0, _Dsize);
//	wcstombs(_Dest, _Source, _Dsize);
//	std::string result = _Dest;
//	delete[]_Dest;
//
//	setlocale(LC_ALL, curLocale.c_str());
//
//	return result;
//}

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

DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_isWXAppInstalledAndSupported);
DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqText);
DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqImg);
DECLARE_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqWeb);

void UWeiXinSDKFunctions::InitJavaFunctions()
{
	//(String text, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqText, "(Ljava/lang/String;Z)V");
	//(String webUrl, String title, String imagePath, String description, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqWeb, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
	//String url, boolean isUrl, boolean isShareToTimeline
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqImg, "(Ljava/lang/String;ZZ)V");
	//boolean(void)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_isWXAppInstalledAndSupported, "()Z");
}

#undef DECLARE_JAVA_METHOD
#undef INIT_JAVA_METHOD

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnWXShareResult(JNIEnv* jenv, jobject thiz, jstring result)
{
	//UE_LOG(LogAndroid, Warning, TEXT("Java_com_epicgames_ue4_GameActivity_nativeOnWXShareResult 1\n"));
	FString Result;

	const char* charsResult = jenv->GetStringUTFChars(result, 0);
	Result = FString(UTF8_TO_TCHAR(charsResult));
	jenv->ReleaseStringUTFChars(result, charsResult);


	//FGraphEventRef EnterBackgroundTask = FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
	//{
	//	//UE_LOG(LogAndroid, Warning, TEXT("Java_com_epicgames_ue4_GameActivity_nativeOnWXShareResult 2\n"));
	//	UWeiXinSDKComponent::onWeiXinShareResultDelegate.Broadcast(Result);
	//}, TStatId(), NULL, ENamedThreads::GameThread);

	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		//UE_LOG(LogAndroid, Warning, TEXT("Java_com_epicgames_ue4_GameActivity_nativeOnWXShareResult 2\n"));
		UWeiXinSDKComponent::onWeiXinShareResultDelegate.Broadcast(Result);
	});
}

#endif

bool UWeiXinSDKFunctions::WeiXinSDK_IsWXAppInstalledAndSupported()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		bool ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_isWXAppInstalledAndSupported);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_isWXAppInstalledAndSupported\n"));

		return ret;
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif
	return false;
}


void UWeiXinSDKFunctions::WeiXinSDK_ShareText(const FString& text, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		//jstring TextArg = Env->NewStringUTF(text.c_str());
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
}

void UWeiXinSDKFunctions::WeiXinSDK_ShareImg(const FString& url, bool isURL, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		//jstring URLArg = Env->NewStringUTF(url.c_str());
		jstring URLArg = Env->NewStringUTF(TCHAR_TO_UTF8(*url));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_sendReqImg, URLArg, (jboolean)isURL, (jboolean)isShareToTimeline);

		Env->DeleteLocalRef(URLArg);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_sendReqImg()\n"));
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif
}

//(String webUrl, String title, String imagePath, String description, boolean isShareToTimeline)
void UWeiXinSDKFunctions::WeiXinSDK_ShareWeb(const FString& url, const FString& title, const FString& imagePath, const FString& description, bool isShareToTimeline)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		//jstring URLArg = Env->NewStringUTF(url.c_str());
		//jstring TitleArg = Env->NewStringUTF(title.c_str());
		//jstring ImagePathArg = Env->NewStringUTF(imagePath.c_str());
		//jstring DescriptionArg = Env->NewStringUTF(description.c_str());

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
