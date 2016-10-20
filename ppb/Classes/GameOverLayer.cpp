#include "GameOverLayer.h"
#include "Contants.h"
#include "Language.h"
#include "Config.h"

GameOverLayer::GameOverLayer()
{

}

GameOverLayer::~GameOverLayer()
{
	CCLog("~GameOverLayer()");
}

bool GameOverLayer::init()
{
	if (!CCLayer::init())
		return false;

	m_uiLayer = UILayer::create();
	addChild(m_uiLayer);

	m_layoutWidget = dynamic_cast<UILayout *>(GUIReader::shareReader()->widgetFromJsonFile("GameOverLayer.json"));
	if (!m_layoutWidget)
	{
		return false;
	}
	m_uiLayer->addWidget(m_layoutWidget);

	m_returnUIImage = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("restartImage"));
	m_returnUIImage->addTouchEventListener(this, toucheventselector(GameOverLayer::pressReturnCallback));
	m_returnUIImage->setTouchEnabled(true);

	m_scoreLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("gameScoreLabel"));
	m_cherryLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("cherryLabel"));

	m_bestScoreLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("bestScoreLabel"));
	m_gameTimesScoreLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("gametimesScoreLabel"));

	int highScore = Config::instance()->highScore();
	int gameTimes = Config::instance()->gameTimes();
	updateGameText(highScore, gameTimes);

	return true;
}

void GameOverLayer::onEnter()
{
	CCLog("GameOverLayer::onEnter()");
	CCLayer::onEnter();

	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	float scalef = s.height/DESIGN_HEIGHT;

	UILabelBMFont *titleLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("titleLabel"));
	if (titleLabel) {
		titleLabel->setPositionY(titleLabel->getPositionY() * scalef);
	}
}

void GameOverLayer::onExit()
{
	CCLog("GameOverLayer::onExit()");
	CCLayer::onExit();
}

void GameOverLayer::pressReturnCallback(CCObject* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("restart button touched!");
			CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_GAVEOVERLAYER_BUTTON_PRESS)));
		}
		break;

	default:
		break;
	}
}

void GameOverLayer::setScore( int score )
{
	m_scoreLabel->setText(
		(CCString::createWithFormat(GET_STRING("SCORES_TOTAL"), score))->getCString());
}

void GameOverLayer::setCherrys( int num )
{
	m_cherryLabel->setText(
		(CCString::createWithFormat("X %d", num))->getCString());
}

void GameOverLayer::updateGameText( int bestScore, int gameTimes )
{
	m_bestScoreLabel->setText(
		(CCString::createWithFormat("%d", bestScore))->getCString());

	m_gameTimesScoreLabel->setText(
		(CCString::createWithFormat("%d", gameTimes))->getCString());
}
