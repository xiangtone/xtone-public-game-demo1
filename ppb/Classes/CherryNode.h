#ifndef __CherryNode_H__
#define __CherryNode_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class CherryNode : public GameObject
{
public:
	CherryNode();
	~CherryNode();

	virtual bool init();
	static CherryNode* create();

	virtual bool collisionWithOther(CCNode *other);

	virtual void updateWithDeltaTime(float dt, CCArray *listObjects);

	virtual CCRect adjustedBoundingBox();
};


#endif //#ifndef __CherryNode_H__