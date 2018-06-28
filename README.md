# WeiXinSDK
ue4 wechat

使用方法
一、将该插件拷贝至工程目录下的Plugins文件夹中

二、APPID修改

[Android] Source/WeiXinSDK/WeiXinSDK_APL.xml 97行

[IOS] APPID修改:
		(1) Source/WeiXinSDK/WeiXinSDK_AdditionalPlistData.xml中22行

		(2) Source/WeiXinSDK/Private/WeiXinSDKFunctions.cpp中108行

三、编辑器设置
[IOS] 打开编辑器 [编辑]->[项目设置]，将修改后Source/WeiXinSDK/WeiXinSDK_AdditionalPlistData.xml文件中的内容拷贝到[平台]->[IOS]->[Extra Plist Data]->[Additional Plist Data]中


图片分享未测试
Android分享返回成功（已测试）
IOS分享返回未测试（切换到微信之后在返回游戏 游戏APP就重启了 目测进程是被系统杀了  因为打开游戏之后进入场景 在返回手机界面 过几秒在打开游戏 游戏又重新启动了  可能是虚幻在后台占用资源太多吧）

