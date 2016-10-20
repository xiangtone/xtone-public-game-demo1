#include "GameObject.h"

USING_NS_CC;

#define		GAME_OBJECT_TEXTURE_TAG		(100)

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

bool GameObject::collisionWithOther( CCNode *other )
{
	return false;
}

bool GameObject::checkNodeRemoval( float playerX )
{
	return false;
}

void GameObject::updateWithDeltaTime( float dt, CCArray *listObjects )
{

}

CCRect GameObject::adjustedBoundingBox()
{
	CCRect r;
	r =  this->boundingBox();

	return r;
}

CCNode* GameObject::createTexture( const char *fileName )
{
	CCSprite *texture = CCSprite::create(fileName);
	texture->setAnchorPoint(ccp(0.5, 0.5));
	texture->setPosition(ccp(texture->getContentSize().width/2, texture->getContentSize().height/2));
	texture->setTag(GAME_OBJECT_TEXTURE_TAG);
	this->setContentSize(texture->getContentSize());

	return (CCNode *)texture;
}

CCNode * GameObject::getObjectTexture()
{
	CCNode *node = this->getChildByTag(GAME_OBJECT_TEXTURE_TAG);

	return node;
}

#ifdef __DEBUG_DRAW__

void GameObject::draw()
{
	ccDrawColor4B(0, 255, 255, 255);

	CCRect r = this->adjustedBoundingBox();

	float xOffset = r.size.width * 0.1f;
	float yOffset = r.size.height * 0.3f;

	ccDrawRect( ccp(xOffset, yOffset), ccp(r.size.width, r.size.height) );

	CHECK_GL_ERROR_DEBUG();
}

#endif //#ifdef __DEBUG_DRAW__
