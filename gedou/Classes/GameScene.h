#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "HudLayer.h"
#include "GameLayer.h"

class GameScene : public CCScene
{
public :
	GameScene();
	bool init();

	void inputUpdate(float dt);

private :
	HudLayer* mHudLayer;
	GameLayer* mGameLayer;
};

#endif