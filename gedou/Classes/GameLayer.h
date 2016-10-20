#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Hero.h"

class GameLayer : public CCLayer
{
public :
	GameLayer();
	bool init();

	CCTMXTiledMap* mTileMap;

	void initTileMap();
	void initHero();

	Hero* mHero;

	//void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvents);

//private :
	CCSpriteBatchNode* mActors;

	void update(float dt);

	void updatePosition(float dt);

	void setViewpointCenter(CCPoint position);

	CCArray* mRobots;
	void initRobots();

	void reorderActors();
	void updateRobots(float dt);
};

#endif