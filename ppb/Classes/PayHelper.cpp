#include "PayHelper.h"

using namespace cocos2d;

PayHelper* PayHelper::m_instance = NULL;

PayHelper::PayHelper()
{
	m_payDelegate = NULL;
	m_retCode = -1;
	m_retResult = 0;
	m_isPayingReturn = false;

	this->onEnter();
	this->onEnterTransitionDidFinish();
}

PayHelper* PayHelper::instance()
{
	if (!m_instance)
		m_instance = new PayHelper;
		
	return m_instance;
}

void PayHelper::initialize()
{
	this->schedule(schedule_selector(PayHelper::updateTimer), 0.3f);
}

void PayHelper::payment(int diamonds, int dollar)
{
	m_isPayingReturn = false;
	startPay(diamonds, dollar);
}

void PayHelper::resultCallback( int code, int diamonds )
{
	CCLOG("PayHelper code = %d", code);
	m_retCode = code;
	m_retResult = diamonds;

	m_isPayingReturn = true;
}

void PayHelper::updateTimer( float dt )
{
	if ( m_isPayingReturn && m_payDelegate ) {
		m_payDelegate->paymentCallback(m_retCode, m_retResult);
		m_retCode = -1;
		m_retResult = 0;

		m_isPayingReturn = false;
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define JAVA_PACKAGE_NAME "com/appu/popobird/JniHelper"

//
//C==>JAVA
//
void startPay(int diamonds, int dollar)
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, JAVA_PACKAGE_NAME, "startPayment", "(II)V");
		
	if(!isHave) {
		CCLog("jni:startPayment not find");
	} else {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, diamonds, dollar);
		CCLog("jni:startPayment ok"); 
	}
}

#ifdef __cplusplus
extern "C" {
#endif
//
//JAVA==>C
//
JNIEXPORT void JNICALL Java_com_appu_popobird_JniHelper_payResultNative(JNIEnv* env, jobject thiz, jint code, jint diamonds)
{
	CCLOG("JAVA-->C code = %d", code);
	CCLOG("JAVA-->C diamonds = %d", diamonds);

	//PayHelper
	PayHelper::instance()->resultCallback(code, diamonds);
}

#ifdef __cplusplus
}
#endif

#else //win32

void startPay(int diamonds, int dollar)
{
	//PayHelper::instance()->resultCallback(0, 5);
}


#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
