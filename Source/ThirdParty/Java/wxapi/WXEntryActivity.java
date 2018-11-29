package com.ChengduWonderVision.BlockMaster.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.tencent.mm.sdk.openapi.BaseReq;
import com.tencent.mm.sdk.openapi.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.epicgames.ue4.GameActivity;


public class WXEntryActivity extends Activity implements IWXAPIEventHandler {

    private static final int RETURN_MSG_TYPE_LOGIN = 1;
    private static final int RETURN_MSG_TYPE_SHARE = 2;

    @Override
    public void onReq(BaseReq baseReq) {

    }

    @Override
    public void onResp(BaseResp resp) {
        //形参resp 有下面两个个属性比较重要
        //1.resp.errCode
        //2.resp.transaction则是在分享数据的时候手动指定的字符创,用来分辨是那次分享(参照4.中req.transaction)
        switch (resp.errCode) { //根据需要的情况进行处理
            case BaseResp.ErrCode.ERR_OK:
            //正确返回
            switch (resp.getType()) {
                case RETURN_MSG_TYPE_LOGIN:
                    break;
                case RETURN_MSG_TYPE_SHARE:
                GameActivity.Get().nativeOnWXShareResult("1", resp.errCode);
                    break;
            }
            break;
            case BaseResp.ErrCode.ERR_USER_CANCEL:
                //用户取消
                GameActivity.Get().nativeOnWXShareResult("0", resp.errCode);
                break;
            case BaseResp.ErrCode.ERR_AUTH_DENIED:
                //认证被否决
                break;
            case BaseResp.ErrCode.ERR_SENT_FAILED:
                //发送失败
                GameActivity.Get().nativeOnWXShareResult("0", resp.errCode);
                break;    
            case BaseResp.ErrCode.ERR_UNSUPPORT:
                //不支持错误
                break;    
            case BaseResp.ErrCode.ERR_COMM:
                //一般错误
                break;       
            default:
                //其他不可名状的情况
                break;
            }
            finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GameActivity.Get().getWXAPI().handleIntent(getIntent(),this);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        GameActivity.Get().getWXAPI().handleIntent(getIntent(),this);
    }
}
