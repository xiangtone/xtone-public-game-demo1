#ifndef __PopupLayer_H__
#define __PopupLayer_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class PopupLayer : public CCLayer
{
public:
	PopupLayer();
	~PopupLayer();

	virtual bool init();
	CREATE_FUNC(PopupLayer);

	virtual void onEnter();
	virtual void onExit();

	void setParam(CCNode *userData) { m_param = userData; };

	void setTipString(const char *tips);
	void setFeeTipsText(const char *feeTips);
	void setConfirmBtnText(const char *text);

	void pressOpenButtonCallback(CCObject* sender, TouchEventType type);
	void pressBackButtonCallback(CCObject* sender, TouchEventType type);
	void back();

	void savePets();

private:
	UILayer *m_uiLayer;
	UILayout *m_layoutWidget;

	UILabel *m_tipsLabel;
	UILabelBMFont *m_openLabel;
	UIImageView *m_backBtn;
	UILabel *m_feeTipsLabel;

	CCNode *m_param;
};


#endif //#ifndef __PopupLayer_H__