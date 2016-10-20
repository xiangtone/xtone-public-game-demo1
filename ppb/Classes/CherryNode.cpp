#include "CherryNode.h"

#define   CHERRY_FILE_NAME   ("cherry.png")

CherryNode::CherryNode()
{

}

CherryNode::~CherryNode()
{

}

bool CherryNode::init()
{
	if (!CCNode::init())
		return false;

	CCNode *cherry = createTexture(CHERRY_FILE_NAME);
	this->addChild(cherry);

	this->setObjectType(kCherryType);

	return true;
}

CherryNode* CherryNode::create()
{
	CherryNode *pRet = new CherryNode();
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

bool CherryNode::collisionWithOther( CCNode *other )
{
	if (other != NULL)
	{
		this->removeFromParent();
	}

	return true;
}

void CherryNode::updateWithDeltaTime( float dt, CCArray *listObjects )
{

}

cocos2d::CCRect CherryNode::adjustedBoundingBox()
{
	CCRect r;

	r = this->boundingBox();

	float xOffset = r.size.width * 0.1f;
	float yOffset = r.size.height * 0.1f;
	float xCropAmount = r.size.width * 0.2f;
	float yCropAmount = r.size.height * 0.2f;

	r = CCRectMake(r.origin.x + xOffset, r.origin.y + yOffset, r.size.width - xCropAmount, r.size.height - yCropAmount);

	return r;
}
