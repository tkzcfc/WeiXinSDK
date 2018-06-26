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
	//(String imagePath, String thumbPath, boolean isShareToTimeline)
	INIT_JAVA_METHOD(AndroidThunkJava_WXSDK_sendReqImg, "(Ljava/lang/String;Ljava/lang/String;Z)V");
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


#if PLATFORM_IOS

#import "WechatDelegate.h"

@implementation WechatDelegate

+ (WechatDelegate*)getInstance
{
	static WechatDelegate* Static_WechatDelegate_Instance = NULL;
	if (Static_WechatDelegate_Instance == NULL)
	{
		Static_WechatDelegate_Instance = [[WechatDelegate alloc] init];
	}
	return Static_WechatDelegate_Instance;
}

-(id)init
{
	self = [super init];

	if (self)
	{
		[[NSNotificationCenter defaultCenter] addObserver: self
												selector : @selector(applicationDidFinishLaunching:)
													name : UIApplicationDidFinishLaunchingNotification
												  object : nil];
	}

	return self;
}

-(void)applicationDidFinishLaunching:(NSNotification *)n
{
	NSDictionary *dLaunchOptionsUrl = n.userInfo[@"UIApplicationLaunchOptionsURLKey"];

	if (!dLaunchOptionsUrl)
	{
		[WXApi registerApp : @"wxbb08167f21b361a2"];
	}
}

/*! @brief 收到一个来自微信的请求，第三方应用程序处理完后调用sendResp向微信发送结果
*
* 收到一个来自微信的请求，异步处理完成后必须调用sendResp发送处理结果给微信。
* 可能收到的请求有GetMessageFromWXReq、ShowMessageFromWXReq等。
* @param req 具体请求内容，是自动释放的
*/
-(void)onReq:(BaseReq*)req
{}



/*! @brief 发送一个sendReq后，收到微信的回应
*
* 收到一个来自微信的处理结果。调用一次sendReq后会收到onResp。
* 可能收到的处理结果有SendMessageToWXResp、SendAuthResp等。
* @param resp具体的回应内容，是自动释放的
*/
-(void)onResp:(BaseResp*)resp
{
	SendMessageToWXResp *sendResp = (SendMessageToWXResp *)resp;
	
	NSString *CodeStr = [NSString stringWithFormat : @"%d",sendResp.errCode];
	FString Result = FString(UTF8_TO_TCHAR([CodeStr UTF8String]));

	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		UWeiXinSDKComponent::onWeiXinShareResultDelegate.Broadcast(Result);
	});
}

@end


void UWeiXinSDKFunctions::InitIOSFunctions()
{
	[WechatDelegate getInstance];
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

#if PLATFORM_IOS
	return [WXApi isWXAppInstalled] && [WXApi isWXAppSupportApi];
#endif
	return false;
}


void UWeiXinSDKFunctions::WeiXinSDK_ShareText(const FString& text, bool isShareToTimeline)
{
	if (!WeiXinSDK_IsWXAppInstalledAndSupported())
		return;
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

#if PLATFORM_IOS

	std::string TextArg = TCHAR_TO_UTF8(*text);

	SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
	req.text = [NSString stringWithUTF8String : TextArg.c_str()];
	req.bText = YES;

	if (isShareToTimeline)
	{
		req.scene = WXSceneTimeline;
	}
	else
	{
		req.scene = WXSceneSession;
	}

	dispatch_async(dispatch_get_main_queue(), ^{
		[WXApi sendReq : req];
	});
#endif
}

void UWeiXinSDKFunctions::WeiXinSDK_ShareImg(const FString& imagePath, const FString& thumbPath, bool isShareToTimeline)
{
	if (!WeiXinSDK_IsWXAppInstalledAndSupported())
		return;
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring ImagePathArg = Env->NewStringUTF(TCHAR_TO_UTF8(*imagePath));
		jstring ThumbPathArg = Env->NewStringUTF(TCHAR_TO_UTF8(*thumbPath));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_WXSDK_sendReqImg, ImagePathArg, ThumbPathArg, (jboolean)isShareToTimeline);

		Env->DeleteLocalRef(ImagePathArg);
		Env->DeleteLocalRef(ThumbPathArg);

		UE_LOG(LogAndroid, Warning, TEXT("I found the java method AndroidThunkJava_WXSDK_sendReqImg()\n"));
	}
	else
	{
		UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
	}
#endif

#if PLATFORM_IOS

	std::string ImagePathArg = TCHAR_TO_UTF8(*imagePath);
	std::string ThumbPathArg = TCHAR_TO_UTF8(*thumbPath);

	WXMediaMessage *message = [WXMediaMessage message];
	
	[message setThumbImage:[UIImage imageNamed:[NSString stringWithUTF8String: ThumbPathArg.c_str()]]];

	WXImageObject *imageObj = [WXImageObject object];

	NSString *filePath = [NSString stringWithUTF8String: ImagePathArg.c_str()];
	imageObj.imageData = [NSData dataWithContentsOfFile:filePath];

	UIImage* image = [UIImage imageWithData:ext.imageData];
	imageObj.imageData = UIImageJPEGRepresentation(image, 0.5);

	message.mediaObject = imageObj;
	
	SendMessageToWXReq* req = [[SendMessageToWXReq alloc] init];
	req.bText   = NO;
	req.message = message;

	if (isShareToTimeline)
	{
		req.scene = WXSceneTimeline;
	}
	else
	{
		req.scene = WXSceneSession;
	}

	dispatch_async(dispatch_get_main_queue(), ^{
		[WXApi sendReq : req];
	});
#endif
}

//(String webUrl, String title, String imagePath, String description, boolean isShareToTimeline)
void UWeiXinSDKFunctions::WeiXinSDK_ShareWeb(const FString& url, const FString& title, const FString& imagePath, const FString& description, bool isShareToTimeline)
{
	if (!WeiXinSDK_IsWXAppInstalledAndSupported())
		return;
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

#if PLATFORM_IOS


	std::string URLArg = TCHAR_TO_UTF8(*url);
	std::string TitleArg = TCHAR_TO_UTF8(*title);
	std::string ImagePathArg = TCHAR_TO_UTF8(*imagePath);
	std::string DescriptionArg = TCHAR_TO_UTF8(*description);

	WXMediaMessage *message = [WXMediaMessage message];
	message.title = [NSString stringWithUTF8String : TitleArg.c_str()];
	message.description = [NSString stringWithUTF8String : DescriptionArg.c_str()];

	[message setThumbImage : [UIImage imageNamed : [NSString stringWithUTF8String : ImagePathArg.c_str()]]];

	WXWebpageObject *webObj = [WXWebpageObject object];
	webObj.webpageUrl = [NSString stringWithUTF8String : URLArg.c_str()];

	message.mediaObject = webObj;

	SendMessageToWXReq* req = [[SendMessageToWXReq alloc] init];
	req.bText = NO;
	req.message = message;

	if (isShareToTimeline)
	{
		req.scene = WXSceneTimeline;
	}
	else
	{
		req.scene = WXSceneSession;
	}

	dispatch_async(dispatch_get_main_queue(), ^{
		[WXApi sendReq : req];
	});
#endif
}
