#ifndef _HUD_LAYER_H_
#define _HUD_LAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Sneaky/SneakyButton.h"
#include "Sneaky/SneakyButtonSkinnedBase.h"
#include "Sneaky/SneakyJoystick.h"
#include "Sneaky/SneakyJoystickSkinnedBase.h"

class HudLayer : public CCLayer
{
public :
	HudLayer();
	bool init();

	SneakyButton* mButtonA;
	SneakyJoystick* mJoystick;

	//void inputUpdate(float dt);
};

#endif