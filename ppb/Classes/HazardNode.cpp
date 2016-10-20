#include "HazardNode.h"

#define   HAZARD_LEFT_FILE_NAME   ("jag_left.png")
#define   HAZARD_RIGHT_FILE_NAME   ("jag_right.png")

HazardNode::HazardNode()
{

}

HazardNode::~HazardNode()
{

}

bool HazardNode::init(HazardNodeType type)
{
	if (!CCNode::init())
		return false;

	std::string fileName;
	if (type == kHazardLeft)
	{
		fileName = HAZARD_LEFT_FILE_NAME;
	}
	else
	{
		fileName = HAZARD_RIGHT_FILE_NAME;
	}

	CCNode *hazard = createTexture(fileName.c_str());
	this->addChild(hazard);

	this->setObjectType(kHazardType);

	return true;
}

HazardNode* HazardNode::create(HazardNodeType type)
{
	HazardNode *pRet = new HazardNode();
	if (pRet && pRet->init(type))
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

bool HazardNode::collisionWithOther( CCNode *other )
{
	/*
	if (other != NULL)
	{
		this->removeFromParent();
	}
	*/

	return true;
}

void HazardNode::updateWithDeltaTime( float dt, CCArray *listObjects )
{

}

cocos2d::CCRect HazardNode::adjustedBoundingBox()
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
