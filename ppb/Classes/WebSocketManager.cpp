#include "WebSocketManager.h"
#include "Contants.h"

USING_NS_CC;
USING_NS_CC_EXT;

WebSocketManager* WebSocketManager::m_instance = NULL;

WebSocketManager::WebSocketManager()
	: m_wsiSendText(NULL)
{
	m_wsiSendText = new WebSocket();
	CCLog("begin socket connect");
	//if (!m_wsiSendText->init(*this, "ws://127.0.0.1:8080/websocket"))
	if (!m_wsiSendText->init(*this, "ws://127.0.0.1:2048/ws"))
	{
		CC_SAFE_DELETE(m_wsiSendText);
	}

}


WebSocketManager::~WebSocketManager()
{
	if (m_wsiSendText)
		m_wsiSendText->close();
}

WebSocketManager* WebSocketManager::instance()
{
	if (m_instance == NULL)
		m_instance = new WebSocketManager();
	return m_instance;
}

void WebSocketManager::onOpen(cocos2d::extension::WebSocket* ws){
	CCLog("Websocket (%p) opened",ws);
	if (ws == m_wsiSendText)
	{
		CCLog(" m_wsiSendText Websocket (%p) opened", ws);
		if (m_wsiSendText->getReadyState() == WebSocket::kStateOpen)
		{
			m_wsiSendText->send("popobird.");
			CCLog("popobird");
		}
		else
		{
			std::string warningStr = "send text websocket instance wasn't ready...";
			CCLog("%s", warningStr.c_str());
		}
	}
}

void WebSocketManager::onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data){
	CCLog("Websocket (%p) onMessage", ws);
	std::string textStr = std::string("response text msg: ") + data.bytes + ", " ;
	std::string str=data.bytes;
	if(str.compare("login")==0){
		CCLog("%s", "post login");
		CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_ONLINE, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_JION)));
	}else if(str.compare("jump")==0){
		//net->jump();
		CCLog("%s", "post jump");
		CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_PRESS_ID, (CCObject *)(CCInteger::create(MSG_ID_POPUPLAYER_JUMP)));
	}else if(str.compare("start")==0){
		CCLog("%s", "post start");
		CCNotificationCenter::sharedNotificationCenter()->postNotification(
				MSG_BUTTON_ONLINE, (CCObject *)(CCInteger::create(MSG_ID_GAME_START)));
	}else{
		CCLog("%s", textStr.c_str());
	}

}

void WebSocketManager::onClose(cocos2d::extension::WebSocket* ws){
	CCLog("Websocket (%p) onClose", ws);
}
void WebSocketManager::onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error){
	CCLog("Websocket (%p) onError", ws);
	CCLog("Error was fired, error code: %d", error);
}

void WebSocketManager::sendMsg(std::string msg){
	if (m_wsiSendText->getReadyState() == WebSocket::kStateOpen)
	{
		m_wsiSendText->send(msg);
		//CCLog("send %s",msg);
	}
	else
	{
		std::string warningStr = "send text websocket instance wasn't ready...";
		CCLog("%s", warningStr.c_str());
	}
}
