#ifndef __AppuSDKInterfaceHelper_H__
#define __AppuSDKInterfaceHelper_H__

#include "cocos2d.h"

struct IAppuSDKInterfaceHelper
{
	IAppuSDKInterfaceHelper() {}
	virtual ~IAppuSDKInterfaceHelper() {}
	virtual void onSuccess() = 0;
	virtual void onFailure() = 0;
};

class AppuSDKInterfaceHelper
{
public:
	AppuSDKInterfaceHelper();
	virtual ~AppuSDKInterfaceHelper() {};
	static AppuSDKInterfaceHelper* instance();

	void init();
	void pay(int price);
	void statistics(int type, int price, const char *desc);
	void exit();

	void addDelegate(IAppuSDKInterfaceHelper *pDelegate){ m_delegate = pDelegate; };
	void removeDelegate() { m_delegate = NULL; };
	
private:
	static AppuSDKInterfaceHelper *m_instance;
	IAppuSDKInterfaceHelper *m_delegate;
};


#ifdef __cplusplus
extern "C" {
#endif
	//C ==> Java
	extern void AppuSDK_onInit();
	extern void AppuSDK_pay(int price);
	extern void AppuSDK_statistics(int type, int price, const char *payDesc);
	extern void AppuSDK_onExit();

	//jni
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
	// Java ==> C
	JNIEXPORT void JNICALL Java_com_appu_sdk_pay_AppuSdk_payResultNative(JNIEnv* env, jobject thiz, jint code, jint diamonds);
#endif  //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#ifdef __cplusplus
}
#endif


#endif //#ifndef __AppuSDKInterfaceHelper_H__