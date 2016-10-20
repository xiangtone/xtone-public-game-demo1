#include "Hero.h"

Hero::Hero()
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//CCSprite::initWithSpriteFrameName("hero_idle_00.png");
	CCSprite::createWithSpriteFrameName("hero_idle_00.png");
	this->retain();
	
	char str[64] = {0};
	CCArray* animFrames = new CCArray(10);

	//Idle
	for (int i = 0; i < 6; ++i) {
		sprintf(str, "hero_idle_%02d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	CCAnimation* idleAnimation = CCAnimation::createWithSpriteFrames(animFrames,1.0f/12.0f);
	mIdleAction = 
		CCRepeatForever::create(CCAnimate::create(idleAnimation));
	mIdleAction->retain();

	//Attack
	animFrames->removeAllObjects();
	for (int i = 0; i < 3; ++i) {
		sprintf(str, "hero_attack_00_%02d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation* attackAnimation =CCAnimation::createWithSpriteFrames(animFrames, 1.0f/24.0f);
	mAttackAction = CCSequence::create(
		CCAnimate::create(attackAnimation), 
		CCCallFunc::create(this, callfunc_selector(Hero::idle)),NULL);
	mAttackAction->retain();

	//Walk
	animFrames->removeAllObjects();
	for (int i = 0; i < 8; ++i) {
		sprintf(str, "hero_walk_%02d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation* walkAnimation = CCAnimation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mWalkAction = 
		CCRepeatForever::create(CCAnimate::create(walkAnimation));
	mWalkAction->retain();

	//Hurt
	animFrames->removeAllObjects();
	for (int i = 0; i < 3; ++i) {
		sprintf(str, "hero_hurt_%02d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation* hurtAnimation = CCAnimation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mHurtAction = CCSequence::create(
		CCAnimate::create(hurtAnimation), 
		CCCallFunc::create(this, callfunc_selector(Hero::idle)),NULL);
	mHurtAction->retain();

	//Knocked out
	animFrames->removeAllObjects();
	for (int i = 0; i < 5; ++i) {
		sprintf(str, "hero_knockout_%02d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation* knockedOutAnimation = CCAnimation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mKnockedOutAction = CCSequence::create(
		CCAnimate::create(knockedOutAnimation), 
		CCBlink::create(2.0f, 10.0f),NULL);
	mKnockedOutAction->retain();

	mCenterToBottom = 39.0f;
	mCenterToSide = 29.0f;
	mHitPoints = 100.0f;
	mDamage = 20.0f;
	mWalkSpeed = 80.0f;

	mHitBox = createBoundingBoxWithOrigin(
		ccp(-mCenterToSide, -mCenterToBottom), 
		CCSizeMake(mCenterToSide * 2, mCenterToBottom * 2));
	mAttackBox = createBoundingBoxWithOrigin(
		ccp(mCenterToSide, -10), CCSizeMake(20, 20));

	this->scheduleUpdate();

	init();
}

bool Hero::init()
{
	ActionSprite::init();

	return true;
}