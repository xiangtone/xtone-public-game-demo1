#ifndef __WallNode_H__
#define __WallNode_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

typedef enum
{
	kWallUp,
	kWallDown
}WallNodeType;

class WallNode : public GameObject
{
public:
	WallNode();
	~WallNode();

	virtual bool init(WallNodeType type);
	static WallNode* create(WallNodeType type);

	virtual bool collisionWithOther(CCNode *other);

	virtual void updateWithDeltaTime(float dt, CCArray *listObjects);

	//virtual CCRect adjustedBoundingBox();
};


#endif //#ifndef __WallNode_H__