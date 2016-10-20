#include "HudLayer.h"

HudLayer::HudLayer()
{
	CCSpriteFrameCache* cache = 
		CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("UI.plist", "UI.png");

	mJoystick = NULL;
	mJoystick = new SneakyJoystick();
	mJoystick->initWithRect(CCRectZero);
	mJoystick->setAutoCenter(true);
	mJoystick->setHasDeadzone(true);
	mJoystick->setDeadRadius(10);
	SneakyJoystickSkinnedBase* jstickSkin = new SneakyJoystickSkinnedBase();
	jstickSkin->autorelease();
	jstickSkin->init();
	jstickSkin->setBackgroundSprite(CCSprite::createWithSpriteFrameName("JoyStick-base.png"));
	jstickSkin->setThumbSprite(CCSprite::createWithSpriteFrameName("JoyStick-thumb.png"));
	//jstickSkin->getThumbSprite()->setScale(0.5f);
	jstickSkin->setPosition(ccp(50, 50));
	jstickSkin->setJoystick(mJoystick);
	this->addChild(jstickSkin);

	mButtonA = NULL;
	mButtonA = new SneakyButton();
	mButtonA->initWithRect(CCRectZero);
	mButtonA->setIsToggleable(false);
	mButtonA->setIsHoldable(true);
	SneakyButtonSkinnedBase* btnASkin = new SneakyButtonSkinnedBase();
	btnASkin->autorelease();
	btnASkin->init();
	btnASkin->setPosition(ccp(430, 50));
	btnASkin->setDefaultSprite(CCSprite::createWithSpriteFrameName("button-default.png"));
	btnASkin->setPressSprite(CCSprite::createWithSpriteFrameName("button-pressed.png"));
	btnASkin->setActivatedSprite(CCSprite::createWithSpriteFrameName("button-activated.png"));
	//btnASkin->setDisabledSprite(CCSprite::createWithSpriteFrameName("button-disabled.png"));
	btnASkin->setButton(mButtonA);
	this->addChild(btnASkin);

	init();
}

bool HudLayer::init()
{
	CCLayer::init();
	return true;
}

/*void HudLayer::inputUpdate(float dt)
{
}*/