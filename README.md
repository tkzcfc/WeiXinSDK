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

四、[Android] 将Source/WeiXinSDK/WeiXinSDK_APL.xml中的包名com.ChengduWonderVision.BlockMaster替换成自己的包名

图片分享未测试
Android分享返回成功（已测试）
IOS分享返回(已测试)
