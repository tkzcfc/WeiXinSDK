
#import "AppDelegate.h"

@interface AppDelegate (WeChatExt)

//被废弃的方法. 但是在低版本中会用到.建议写上
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

//被废弃的方法. 但是在低版本中会用到.建议写上
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url;
  
//新的方法
-(BOOL) application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options;

@end
