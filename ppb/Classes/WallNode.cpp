#include "WallNode.h"

#define   UP_WALL_FILE_NAME		("bg_hazard_up.png")
#define   DOWN_WALL_FILE_NAME   ("bg_hazard_down.png")

WallNode::WallNode()
{

}

WallNode::~WallNode()
{

}

bool WallNode::init(WallNodeType type)
{
	if (!CCNode::init())
		return false;

	std::string fileName;

	if (type == kWallUp)
	{
		fileName = UP_WALL_FILE_NAME;
	}
	else
	{
		fileName = DOWN_WALL_FILE_NAME;
	}

	CCNode *wall = createTexture(fileName.c_str());
	this->addChild(wall);

	this->setObjectType(kWallType);

	return true;
}

WallNode* WallNode::create(WallNodeType type)
{
	WallNode *pRet = new WallNode();
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

bool WallNode::collisionWithOther( CCNode *other )
{
	return true;
}

void WallNode::updateWithDeltaTime( float dt, CCArray *listObjects )
{

}
