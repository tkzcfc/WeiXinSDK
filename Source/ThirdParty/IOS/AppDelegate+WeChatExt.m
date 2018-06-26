#import "AppDelegate+WeChatExt.h"
#import "WechatDelegate.h"

@implementation AppDelegate (WeChatExt)

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
	return [WXApi handleOpenURL:url delegate:[WechatDelegate getInstance]];
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
	return [WXApi handleOpenURL:url delegate:[WechatDelegate getInstance]];
}

- (BOOL) application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options
{
    return [WXApi handleOpenURL:url delegate:[WechatDelegate getInstance]];
}

@end
