#ifndef __BubbleNode_H__
#define __BubbleNode_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class BubbleNode : public GameObject
{
public:
	BubbleNode();
	~BubbleNode();

	virtual bool init();

	static BubbleNode* create();

	virtual bool collisionWithOther(CCNode *other);

	virtual void updateWithDeltaTime(float dt, CCArray *listObjects);

	//virtual CCRect adjustedBoundingBox();
};


#endif //#ifndef __BubbleNode_H__