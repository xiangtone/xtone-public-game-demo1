#ifndef __PayHelper_H__
#define __PayHelper_H__

#include "cocos2d.h"

#define 	PAY_REQUEST_OK 		(0)
#define		PAY_RESULT_FAIL		(1)

struct IPayHelper
{
	IPayHelper() {}
	virtual ~IPayHelper() {}
	virtual void paymentCallback(int retcode, int diamonds) = 0;
};

class PayHelper : public cocos2d::CCNode
{
public:
	PayHelper();
	virtual ~PayHelper() {};
	static PayHelper* instance();

	void initialize();
	void payment(int diamonds, int dollar);
	void addDelegate(IPayHelper *pDelegate){ m_payDelegate = pDelegate; };
	void removeDelegate() { m_payDelegate = NULL; };
	
	void resultCallback(int code, int diamonds);

	void updateTimer(float dt);

private:
	static PayHelper *m_instance;
	IPayHelper *m_payDelegate;
	int m_retCode;
	int m_retResult;
	bool m_isPayingReturn;
};


#ifdef __cplusplus
extern "C" {
#endif

extern void startPay(int diamonds, int dollar);

//jni
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

JNIEXPORT void JNICALL Java_com_appu_popobird_JniHelper_payResultNative(JNIEnv* env, jobject thiz, jint code, jint diamonds);
#endif  //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#ifdef __cplusplus
}
#endif


#endif //#ifndef __PayHelper_H__