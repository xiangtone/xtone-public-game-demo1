#ifndef __GameOverLayer_H__
#define __GameOverLayer_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class GameOverLayer : public CCLayer
{
public:
	GameOverLayer();
	~GameOverLayer();

	virtual bool init();
	CREATE_FUNC(GameOverLayer);

	void setScore(int score);
	void setCherrys(int num);
	void updateGameText(int bestScore, int gameTimes);

	virtual void onEnter();
	virtual void onExit();

	void pressReturnCallback(CCObject* sender, TouchEventType type);

private:
	UILayer *m_uiLayer;
	UILayout *m_layoutWidget;
	UIImageView *m_returnUIImage;
	UILabelBMFont *m_scoreLabel;
	UILabelBMFont *m_cherryLabel;

	UILabelBMFont *m_bestScoreLabel;
	UILabelBMFont *m_gameTimesScoreLabel;
};


#endif //#ifndef __GameOverLayer_H__