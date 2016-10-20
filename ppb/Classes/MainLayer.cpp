#include "MainLayer.h"
#include "Contants.h"
#include "Language.h"
#include "Config.h"
#include "PopupLayer.h"
#include "AudioManager.h"
#include "WebSocketManager.h"
MainLayer::MainLayer()
{
	connect=false;
}

MainLayer::~MainLayer()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUTTON_ONLINE);
}

bool MainLayer::init()
{
	if (!CCLayer::init())
		return false;

	enableKeypad();

	m_uiLayer = UILayer::create();
	addChild(m_uiLayer);

	m_layoutWidget = dynamic_cast<UILayout *>(GUIReader::shareReader()->widgetFromJsonFile("StartLayer.json"));
	if (!m_layoutWidget)
	{
		return false;
	}
	m_uiLayer->addWidget(m_layoutWidget);

	m_petUIImage = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("petImage"));
	m_petUIImage->addTouchEventListener(this, toucheventselector(MainLayer::pressPetImageCallback));
	m_petUIImage->setTouchEnabled(true);

	m_centerPos = m_petUIImage->getPosition();

	m_startUIImage = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("startbutton"));
	m_startUIImage->addTouchEventListener(this, toucheventselector(MainLayer::pressStartButtonCallback));
	m_startUIImage->setTouchEnabled(true);

	m_onlineUIImage = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("onlinebutton"));
	m_onlineUIImage->addTouchEventListener(this, toucheventselector(MainLayer::pressOnlineButtonCallback));
	m_onlineUIImage->setTouchEnabled(true);

	m_changeLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("changeLabel"));
	m_changeLabel->addTouchEventListener(this, toucheventselector(MainLayer::pressChangeRoleCallback));
	m_changeLabel->setTouchEnabled(true);

	m_priceTipsLabel = dynamic_cast<UILabel *>(m_uiLayer->getWidgetByName("feeLabel"));
	m_cherryLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("cherryLabel"));
	m_bestScoreLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("bestScoreLabel"));
	m_gameTimesScoreLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("gametimesScoreLabel"));

	m_petType = (PetType)Config::instance()->petType();
	setPriceTipText(m_petType);
	updatePetImage(m_petType);

	int cherryNum = Config::instance()->cherryNum();
	int gameTimes = Config::instance()->gameTimes();
	int highScore = Config::instance()->highScore();

	updateMainText(cherryNum, gameTimes, highScore);
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
		callfuncO_selector(MainLayer::notifyHandle), MSG_BUTTON_ONLINE, NULL);
	WebSocketManager::instance();
	
	return true;
}

void MainLayer::onEnter()
{
	CCLog("MainLayer::onEnter()");
	CCLayer::onEnter();

	doFloatAction();
	doBlinkAction();

	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	float scalef = s.height/DESIGN_HEIGHT;

	UILabelBMFont *titleLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("titleLabel"));
	if (titleLabel) {
		titleLabel->setPositionY(titleLabel->getPositionY() * scalef);
	}

	UILabelBMFont *cherryLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("cherryLabel"));
	if (cherryLabel) {
		cherryLabel->setPositionY(cherryLabel->getPositionY() * scalef);
	}

	UIImageView *cherryIcon = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("cherryIcon"));
	if (cherryIcon) {
		cherryIcon->setPositionY(cherryIcon->getPositionY() * scalef);
	}
}

void MainLayer::onExit()
{
	CCLog("MainLayer::onExit()");
	CCLayer::onExit();
}

void MainLayer::pressPetImageCallback(CCObject* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("pet touched!");
			switchPets();
		}
		break;

	default:
		break;
	}
}

void MainLayer::pressStartButtonCallback(CCObject* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("start button touched!");
			startGame();
		}
		break;

	default:
		break;
	}
}

void MainLayer::pressChangeRoleCallback( CCObject* sender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("change role touched!");
			switchPets();
		}
		break;

	default:
		break;
	}
}

void MainLayer::pressOnlineButtonCallback( CCObject* sender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("online button touched!");
			
			if(connect){
				WebSocketManager::instance()->sendMsg("start");
				startGame();
			}else{
				WebSocketManager::instance()->sendMsg("login");
			}
		}
		break;

	default:
		break;
	}
}

void MainLayer::doFloatAction()
{
	CCActionInterval *moveBy = CCMoveBy::create(2.f, ccp(0, 70));
	CCEaseSineInOut *sineInOut = CCEaseSineInOut::create(moveBy);
	CCActionInterval *seq = CCSequence::create(sineInOut, sineInOut->reverse(), NULL);
	CCRepeatForever *repeat = CCRepeatForever::create(seq);

	m_petUIImage->runAction(repeat);
}

void MainLayer::doBlinkAction()
{
	CCBlink *blink = CCBlink::create(1.5f, 1);
	CCRepeatForever *repeat = CCRepeatForever::create(blink);

	m_changeLabel->runAction(repeat);
}

void MainLayer::switchPets()
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	CCSize petSize = m_petUIImage->getContentSize();

	m_petUIImage->setTouchEnabled(false);
	m_changeLabel->setTouchEnabled(false);

	float moveTime = 0.25f;

	CCActionInterval *moveOut = CCMoveTo::create(moveTime, ccp(-petSize.width/2, m_centerPos.y));
	m_petUIImage->runAction(moveOut);

	int pet_type = m_petType;
	if (++pet_type >= PET_MAX)
	{
		m_petType = PET01;
	}
	else
	{
		m_petType = (PetType)pet_type;
	}

	Config::instance()->savePetType(m_petType);

	setPriceTipText(m_petType);

	CCString *str = CCString::createWithFormat("%s_01.png", s_pet_list[m_petType]);
	CCSprite *moveInPet = CCSprite::createWithSpriteFrameName(str->getCString());
	moveInPet->setAnchorPoint(ccp(0.5, 0.5));
	moveInPet->setPosition(ccp(s.width + moveInPet->getContentSize().width/2, m_centerPos.y));
	this->addChild(moveInPet);

	CCActionInterval *moveIn = CCMoveTo::create(moveTime, m_centerPos);
	CCCallFuncN *callfunN = CCCallFuncN::create(this, callfuncN_selector(MainLayer::switchCallback));
	CCActionInterval *seq = CCSequence::create(CCDelayTime::create(moveTime), moveIn, callfunN, NULL);
	moveInPet->runAction(seq);

	AudioManager::instance()->playEffect(soundeffect_swap);
}

void MainLayer::updatePetImage( PetType type )
{
	CCString *str = CCString::createWithFormat("%s_01.png", s_pet_list[type]);
	m_petUIImage->loadTexture(str->getCString(), UI_TEX_TYPE_PLIST);
}

void MainLayer::switchCallback( CCNode *pSender )
{
	if (pSender != NULL)
	{
		updatePetImage(m_petType);
		m_petUIImage->setPosition(m_centerPos);
		pSender->removeFromParent();

		m_petUIImage->setTouchEnabled(true);
		m_changeLabel->setTouchEnabled(true);
	}
}

void MainLayer::setPriceTipText( PetType type )
{
	bool isOpen = false;
	isOpen = isPetOpen(type);

	if (type == PET01 || isOpen)
	{
		m_priceTipsLabel->setVisible(false);
	}
	else
	{
		m_priceTipsLabel->setVisible(true);
	}

	m_priceTipsLabel->setText(
		(CCString::createWithFormat(GET_STRING("PET_PRICE_TIPS"), s_pet_price[type]))->getCString());
}

void MainLayer::updateMainText( int cherry, int times, int bestScore )
{
	m_cherryLabel->setText(
		(CCString::createWithFormat("X %d", cherry))->getCString());

	m_bestScoreLabel->setText(
		(CCString::createWithFormat("%d", bestScore))->getCString());

	m_gameTimesScoreLabel->setText(
		(CCString::createWithFormat("%d", times))->getCString());
}

void MainLayer::startGame()
{
	if (isPetOpen(m_petType))
	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification(
			MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_STARTLAYER_BUTTON_PRESS)));
	}
	else
	{
		int price = s_pet_price[m_petType];
		int cherryNum = Config::instance()->cherryNum();
		if (cherryNum < price)
		{
			m_uiLayer->setTouchEnabled(false);

			PopupLayer *popLayer = PopupLayer::create();
			this->addChild(popLayer);

			popLayer->setParam(m_uiLayer);
		}
		else
		{
			Config::instance()->useCherry(price);

			CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_STARTLAYER_BUTTON_PRESS)));
		}
	}
}

bool MainLayer::isPetOpen(PetType pet)
{
	std::string s = Config::instance()->petString();

	bool isOpen = false;
	char boolChar = s[pet];
	if (boolChar == '1')
	{
		isOpen = true;
	}
	else
	{
		isOpen = false;
	}

	return isOpen;
}

void MainLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}

void MainLayer::notifyHandle(CCObject *obj)
{
	int id = ((CCInteger *)obj)->getValue();
	switch (id)
	{
	case MSG_ID_POPUPLAYER_JION:
		{
			CCLog("%s","get notify login");
			connect=true;
			CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_JION)));
			break;
		}
	case MSG_ID_GAME_START:
		{
			CCLog("%s","get notify start");
			CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_JION)));
			startGame();
		}
		
	default:
		break;
	}
}