#ifndef __MainLayer_H__
#define __MainLayer_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Contants.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class MainLayer : public CCLayer
{
public:
	MainLayer();
	~MainLayer();

	virtual bool init();
	CREATE_FUNC(MainLayer);

	virtual void onEnter();
	virtual void onExit();

	void doFloatAction();
	void doBlinkAction();
	void switchPets();
	void switchCallback(CCNode *pSender);
	void setPriceTipText(PetType type);
	void updatePetImage(PetType type);
	void startGame();
	bool isPetOpen(PetType pet);

	void updateMainText(int cherry, int times, int bestScore);

	void pressPetImageCallback(CCObject* sender, TouchEventType type);
	void pressStartButtonCallback(CCObject* sender, TouchEventType type);
	void pressChangeRoleCallback(CCObject* sender, TouchEventType type);
	void pressOnlineButtonCallback(CCObject* sender, TouchEventType type);

	void enableKeypad()
	{
		this->setKeypadEnabled(true);
	}

	void disableKeypad()
	{
		this->setKeypadEnabled(false);
	}

	virtual void keyBackClicked();

	void notifyHandle(CCObject *obj);

private:
	UILayer *m_uiLayer;
	UILayout *m_layoutWidget;
	UIImageView *m_petUIImage;
	UIImageView *m_startUIImage;
	UIImageView *m_onlineUIImage;
	UILabelBMFont *m_changeLabel;

	UILabelBMFont *m_cherryLabel;
	UILabelBMFont *m_bestScoreLabel;
	UILabelBMFont *m_gameTimesScoreLabel;

	UILabel *m_priceTipsLabel;
	PetType m_petType;
	CCPoint m_centerPos;
	bool connect;
};


#endif //#ifndef __MainLayer_H__