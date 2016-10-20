#ifndef __GameObject_H__
#define __GameObject_H__

#include "cocos2d.h"
#include "Contants.h"

typedef enum
{
	kGameObjectNone,
	kWallType,
	kHazardType,
	kCherryType,
	kBubbleType
}GameObjectType;

class GameObject : public cocos2d::CCNode
{
public:
	GameObject();
	virtual ~GameObject();

	// Called when a player collides with the game object
	virtual bool collisionWithOther(CCNode *other);

	// Called every frame to see if the game object should be removed from the scene
	virtual bool checkNodeRemoval(float playerX);

	//called every frame
	virtual void updateWithDeltaTime(float dt, cocos2d::CCArray *listObjects);

	//adjust collision box from a texture
	virtual cocos2d::CCRect adjustedBoundingBox();

	void setObjectType(GameObjectType type) { m_objectType = type; };
	GameObjectType getObjectType() { return m_objectType; };

	virtual CCNode* createTexture(const char *fileName);

	CCNode *getObjectTexture();

#ifdef __DEBUG_DRAW__
	//debug draw
	virtual void draw();
#endif

protected:
	GameObjectType m_objectType;
};

#endif //#ifndef __GameObject_H__