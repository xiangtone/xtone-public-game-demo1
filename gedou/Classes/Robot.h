#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "ActionSprite.h"

class Robot : public ActionSprite
{
public :
	Robot();
	bool init();

	float mNextDecisionTime;
};

#endif