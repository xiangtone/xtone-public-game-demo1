#include "BubbleNode.h"

#define   BUBBLE_FILE_NAME   ("bubble.png")

BubbleNode::BubbleNode()
{

}

BubbleNode::~BubbleNode()
{

}

bool BubbleNode::init()
{
	if (!CCNode::init())
		return false;

	CCNode *bubble = createTexture(BUBBLE_FILE_NAME);
	this->addChild(bubble);

	this->setObjectType(kBubbleType);

	return true;
}

BubbleNode* BubbleNode::create()
{
	BubbleNode *pRet = new BubbleNode();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool BubbleNode::collisionWithOther( CCNode *other )
{
	if (other != NULL)
	{
		this->removeFromParent();
	}

	return true;
}

void BubbleNode::updateWithDeltaTime( float dt, CCArray *listObjects )
{

}
