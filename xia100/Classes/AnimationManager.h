#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "cocos2d.h"
#include "Singleton.h"
USING_NS_CC;

enum{
	aFall,
	aOnNormalBoardLeft,
	aOnNormalBoardRight,
	aOnStarBoardLeft,
	aOnStarBoardRight,
	aRollingBoard,
	aRotateLeftBoard,
	aRotateRightBoard,
	aSpringBoard,
	aOnRollingBoard
};


class AnimationManager : public Singleton<AnimationManager>{
public:
	AnimationManager();
	~AnimationManager();
	//��ʼ������ģ�滺���
	bool initAnimationMap();
	//�������ֵõ�һ������ģ��
	CCAnimation* getAnimation(int key);
	//����һ������ʵ��
	CCAnimate* createAnimate(int key);
	//����һ������ʵ��
	CCAnimate* createAnimate(const char* key);
private:
	CCSpriteBatchNode *_gameBatchNode;
	CCAnimation* createBobFallAnimation();
	CCAnimation* createBobOnNormalBoardLeftAnimation();
	CCAnimation* createBobOnNormalBoardRightAnimation();
	CCAnimation* createBobOnStarBoardLeftAnimation();
	CCAnimation* createBobOnStarBoardRightAnimation();
	CCAnimation* createRollingBoardAnimation();
	CCAnimation* createRotateLeftBoardAnimation();
	CCAnimation* createRotateRightBoardAnimation();
	CCAnimation* createSpringBoardAnimation();
};

//���嶯��������ʵ���ı���
#define sAnimationMgr AnimationManager::instance()

#endif