#include "AppuSDKInterfaceHelper.h"

using namespace cocos2d;

AppuSDKInterfaceHelper* AppuSDKInterfaceHelper::m_instance = NULL;

AppuSDKInterfaceHelper::AppuSDKInterfaceHelper()
{
	
}

AppuSDKInterfaceHelper* AppuSDKInterfaceHelper::instance()
{
	if (!m_instance)
		m_instance = new AppuSDKInterfaceHelper;

	return m_instance;
}

void AppuSDKInterfaceHelper::init()
{
	AppuSDK_onInit();
}

void AppuSDKInterfaceHelper::pay(int price)
{
	AppuSDK_pay(price);
}

void AppuSDKInterfaceHelper::statistics(int type, int price, const char *desc)
{
	AppuSDK_statistics(type, price, desc);
}

void AppuSDKInterfaceHelper::exit()
{
	AppuSDK_onExit();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define JAVA_PACKAGE_NAME "com/appu/sdk/pay/AppuSdk"

#ifdef __cplusplus
extern "C" {
#endif

//
//C==>JAVA
//
void AppuSDK_onInit()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, JAVA_PACKAGE_NAME, "init", "()V");

	if(!isHave) {
		CCLog("jni:init not find");
	} else {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
		CCLog("jni:init ok"); 
	}
}

void AppuSDK_pay(int price)
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, JAVA_PACKAGE_NAME, "pay", "(I)V");

	if(!isHave) {
		CCLog("jni:pay not find");
	} else {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, price);
		minfo.env->DeleteLocalRef(minfo.classID);
		CCLog("jni:pay ok"); 
	}
}

void AppuSDK_statistics( int type, int price, const char *payDesc )
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, JAVA_PACKAGE_NAME, "statistics", "(IILjava/lang/String;)V");

	jstring strArgPayDesc = minfo.env->NewStringUTF(payDesc);

	if(!isHave) {
		CCLog("jni:statistics not find");
	} else {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, type, price, strArgPayDesc);

		//release
		minfo.env->DeleteLocalRef(strArgPayDesc);
		minfo.env->DeleteLocalRef(minfo.classID);
		CCLog("jni:statistics ok"); 
	}
}

void AppuSDK_onExit()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, JAVA_PACKAGE_NAME, "onExit", "()V");

	if(!isHave) {
		CCLog("jni:onExit not find");
	} else {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
		CCLog("jni:onExit ok"); 
	}
}

//
//JAVA==>C
//
JNIEXPORT void JNICALL Java_com_appu_sdk_pay_AppuSdk_payResultNative(JNIEnv* env, jobject thiz, jint code, jint diamonds)
{
	CCLOG("JAVA-->C code = %d", code);
	CCLOG("JAVA-->C diamonds = %d", diamonds);

	//PayHelper
	//PayHelper::instance()->resultCallback(code, diamonds);
}

#ifdef __cplusplus
}
#endif

#else //win32

void AppuSDK_onInit()
{

}

void AppuSDK_pay(int price)
{

}

void AppuSDK_statistics( int type, int price, const char *payDesc )
{

}

void AppuSDK_onExit()
{

}


#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
