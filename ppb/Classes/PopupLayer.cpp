#include "PopupLayer.h"
#include "Contants.h"
#include "Config.h"

PopupLayer::PopupLayer()
{
	m_openLabel = NULL;
	m_tipsLabel = NULL;
	m_param = NULL;
}

PopupLayer::~PopupLayer()
{

}

bool PopupLayer::init()
{
	if (!CCLayer::init())
		return false;

	m_uiLayer = UILayer::create();
	addChild(m_uiLayer);

	CCLayerColor *bgMask = CCLayerColor::create(ccc4(65, 65, 65, 100));
	bgMask->setAnchorPoint(ccp(0.5, 0.5));
	bgMask->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
	bgMask->ignoreAnchorPointForPosition(false);
	this->addChild(bgMask, -10);

	m_layoutWidget = dynamic_cast<UILayout *>(GUIReader::shareReader()->widgetFromJsonFile("PopupLayer.json"));
	if (!m_layoutWidget)
	{
		return false;
	}
	m_uiLayer->addWidget(m_layoutWidget);

	m_openLabel = dynamic_cast<UILabelBMFont *>(m_uiLayer->getWidgetByName("confirmBtn"));
	m_openLabel->addTouchEventListener(this, toucheventselector(PopupLayer::pressOpenButtonCallback));
	m_openLabel->setTouchEnabled(true);

	m_tipsLabel = dynamic_cast<UILabel *>(m_uiLayer->getWidgetByName("tipsLabel"));
	m_feeTipsLabel = dynamic_cast<UILabel *>(m_uiLayer->getWidgetByName("feeTipsLabel"));

	m_backBtn = dynamic_cast<UIImageView *>(m_uiLayer->getWidgetByName("backBtn"));
	m_backBtn->addTouchEventListener(this, toucheventselector(PopupLayer::pressBackButtonCallback));
	m_backBtn->setTouchEnabled(true);

	return true;
}

void PopupLayer::onEnter()
{
	CCLog("PopupLayer::onEnter()");
	CCLayer::onEnter();
}

void PopupLayer::onExit()
{
	CCLog("PopupLayer::onExit()");
	CCLayer::onExit();
}

void PopupLayer::setTipString( const char *tips )
{
	if (m_tipsLabel) {
		m_tipsLabel->setText(tips);
	}
}

void PopupLayer::setFeeTipsText( const char *feeTips )
{
	if (m_feeTipsLabel) {
		m_feeTipsLabel->setText(feeTips);
	}
}

void PopupLayer::setConfirmBtnText( const char *text )
{
	if (m_openLabel) {
		m_openLabel->setText(text);
	}
}

void PopupLayer::pressOpenButtonCallback(CCObject* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("open button touched!");
			if (this->getTag() == GAME_PLAY_POPUP_LAYER_TAG)
			{
				CCNotificationCenter::sharedNotificationCenter()->postNotification(
					MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_GAMEPLAYING_BUTTON_PRESS)));
				back();
			}
			else
			{
				CCNotificationCenter::sharedNotificationCenter()->postNotification(
					MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_BUTTON_PRESS)));
				savePets();
			}
		}
		break;

	default:
		break;
	}
}

void PopupLayer::pressBackButtonCallback( CCObject* sender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCLog("back button touched!");
			back();
		}
		break;

	default:
		break;
	}
}

void PopupLayer::back()
{
	((CCLayer *)m_param)->setTouchEnabled(true);
	if (this->getTag() == GAME_PLAY_POPUP_LAYER_TAG)
	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification(
			MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_BACKTOGAME)));
	}

	this->removeFromParent();
}

void PopupLayer::savePets()
{
	int petType = Config::instance()->petType();
	std::string s = Config::instance()->petString();

	for (int i = 0; i < PET_MAX; ++i) {
		if (petType == i) {
			s[i] = '1';
			break;
		}
	}

	Config::instance()->savePetString(s);
}
