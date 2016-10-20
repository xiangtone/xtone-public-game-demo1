#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"
#include "PopoBirdSprite.h"
#include <vector>
#include "WebSocketManager.h"
USING_NS_CC;

class WallNode;

typedef enum
{
	kGameStateNone,
	kGameStatePlaying,
	kGameStatePause,
	KGameStateGameOver,
	KGameStateMax
}GameStateType;

class GameScene : public cocos2d::CCLayer, public PopoBirdDelegate
{
public:
	GameScene();
	~GameScene();
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);

	void initSpeets();
	void createBackground();
	void createPopoBird();
	void startGamePlaying();

	void generateHazard(int dirType, float y);
	void removeAllHazard();
	void generateCherry();
	void randomSetHazard(int dirType);
	std::vector<int> randomYPos(int num);
	void getRandomPosition(CCSize s, int &randomX, int &randomY);
	void generateBubble();

	void generateCollisionObject();

	void updateScore();
	void doAddCherryScoreAction(const CCPoint &pos);

	void updateBubbleSecondLabel(int second);

	void addMainLayer();
	void showPopupLayer(const char *tips);
	void showGameOverLayer();

	void notifyHandle(CCObject* obj);

	//game main loop
	virtual void update(float delta);

	//touchs
	virtual void registerWithTouchDispatcher(void);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//delegate
	virtual void didContact(CCNode *objectA, GameObject *objectB);
	virtual void didGameOver();
	virtual void turnDirectionLeft();
	virtual void turnDirectionRight();
	virtual void didShowTips();
	virtual void updateBubbleSecond(int sec);

private:
	CCNode *m_backgroundNode;

	CCSize m_visibleSize;
	CCPoint m_origin;

	PopoBirdSprite *m_popoBird;
	PopoBirdSprite *m_popoBirdNet;

	int m_scores;
	int m_cheerys;
	CCLabelBMFont *m_scoreLabel;

	int m_delayCount;
	int m_bubbleTimes;

	CCArray *m_collisionNodes;

	GameStateType m_gameState;

	//wall node
	WallNode *m_wallUpNode;
	WallNode *m_wallDownNode;

	//bubble tips
	CCLabelTTF *m_bubbleTips;

	bool online;
};

#endif // __GameScene_H__
