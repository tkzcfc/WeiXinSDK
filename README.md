# WeiXinSDK
ue4 wechat

引擎版本:4.20

使用方法
一、将该插件拷贝至工程目录下的Plugins文件夹中

二、APPID修改

[Android] Source/WeiXinSDK/WeiXinSDK_APL.xml 93行

[IOS] APPID修改:
		(1) Source/WeiXinSDK/WeiXinSDK_AdditionalPlistData.xml中23行

		(2) Source/WeiXinSDK/Private/WeiXinSDKFunctions.cpp中104行

三、编辑器设置
[IOS] 打开编辑器 [编辑]->[项目设置]，将修改后Source/WeiXinSDK/WeiXinSDK_AdditionalPlistData.xml文件中的内容拷贝到[平台]->[IOS]->[Extra Plist Data]->[Additional Plist Data]中
	注 ：1)这一步可以类似安卓写xml配置文件，IOS的有点复杂，有兴趣的可以自己去试试)
		 2)QQSDK插件也是类似的做法，要同时使用这两个插件要将两个xml文件的元素合一下,合过的代码我放在工程Resources/QQ&WX.xml中的
		 

四、[Android] 将Source/WeiXinSDK/WeiXinSDK_APL.xml中的包名com.ChengduWonderVision.BlockMaster替换成自己的包名

五、要分享截屏的截屏功能可以参考 https://github.com/getsetgames/Sharing
