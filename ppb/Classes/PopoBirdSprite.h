#ifndef __PopoBirdSprite_H__
#define __PopoBirdSprite_H__

#include "cocos2d.h"
#include "Contants.h"

USING_NS_CC;

class GameObject;

static const ccColor3B s_particle_color[PET_MAX] = {
	ccc3(0, 182, 233),
	ccc3(231, 73, 156),
	ccc3(247, 235, 8),
	ccc3(115, 215, 57),
	ccc3(181, 227, 132)
};

typedef enum
{
	kPopoBirdStateNone,
	kPopoBirdStatePlay,
	kPopoBirdStateLeftMoving,
	kPopoBirdStateRightMoving,
	kPopoBirdStateDead,
	kPopoBirdStateMax
}BirdStateType;

struct PopoBirdDelegate
{
	virtual void didContact(CCNode *objectA, GameObject *objectB) = 0;
	virtual void didGameOver() = 0;
	virtual void turnDirectionLeft() = 0;
	virtual void turnDirectionRight() = 0;
	virtual void didShowTips() = 0;
	virtual void updateBubbleSecond(int sec) = 0;
};

class PopoBirdSprite : public cocos2d::CCSprite
{
public:
	PopoBirdSprite();
	~PopoBirdSprite();

	virtual bool init();
	CREATE_FUNC(PopoBirdSprite);
	virtual void onEnter();

	void setDelegate(PopoBirdDelegate *pDelegate) { m_delegate = pDelegate; };
	void updateWithDeltaTime(float dt, CCArray *listObjects);

	void jump();
	void playFlyAnimation();
	void push();
	void dead();
	void deadActionCallback();

	void setRole(PetType type);

	void addBubble();
	bool hasBubble() { return m_hasBubble; };
	void bubbleTimeHandler(float dt);

	void showTips();

	void addLife();
#ifdef __DEBUG_DRAW__
	virtual void draw();
#endif

private:
	void loadAnimation(PetType type);
	void addParticle();
	void animateOverHandler();
	bool collisionWithOther(CCNode *other);
	void checkCollisions(CCArray *listObjects);
	cocos2d::CCRect adjustedBoundingBox();

private:
	CCPoint m_velocity;
	CCPoint m_gravity;

	float m_screenWidth;
	float m_screenHeight;

	PopoBirdDelegate *m_delegate;

	BirdStateType m_state;
	PetType m_roleType;

	CCAnimation *m_flyAnimation;

	bool m_hasBubble;
	unsigned int m_jumpSoundId;

	int m_bubbleTimeCnt;
	//int life;
};

#endif // __PopoBirdSprite_H__
