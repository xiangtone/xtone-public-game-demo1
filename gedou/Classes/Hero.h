#ifndef _HERO_H_
#define _HERO_H_

#include "ActionSprite.h"

class Hero : public ActionSprite
{
public :
	Hero();
	bool init();
};

#endif