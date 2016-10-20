#ifndef __HazardNode_H__
#define __HazardNode_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

typedef enum
{
	kHazardLeft,
	kHazardRight
}HazardNodeType;

class HazardNode : public GameObject
{
public:
	HazardNode();
	~HazardNode();

	virtual bool init(HazardNodeType type);
	static HazardNode* create(HazardNodeType type);

	virtual bool collisionWithOther(CCNode *other);

	virtual void updateWithDeltaTime(float dt, CCArray *listObjects);

	virtual CCRect adjustedBoundingBox();
};


#endif //#ifndef __HazardNode_H__