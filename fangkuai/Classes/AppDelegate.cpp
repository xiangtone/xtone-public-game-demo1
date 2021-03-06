#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
HelloWorld *g_layer;  

void Win32SetKeyLayer(HelloWorld *layer)  
{  
	g_layer = layer;  
}  

void Win32KeyHook( UINT message,WPARAM wParam, LPARAM lParam )  
{  
	//CCLog("Win32KeyHook message %d wParam %d lParam %d", message, wParam, lParam);  
	if (g_layer)  
		g_layer->onWin32KeyEvent(message, wParam, lParam);  
}  
#endif

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)    
	pEGLView->setAccelerometerKeyHook(Win32KeyHook);	//加入键盘处理代码
	#endif

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
