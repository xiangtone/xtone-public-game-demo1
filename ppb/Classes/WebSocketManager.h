#ifndef __WebSocketManager_H__
#define __WebSocketManager_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/WebSocket.h"
#include "PopoBirdSprite.h"

class WebSocketManager 
	: public cocos2d::extension::WebSocket::Delegate
{
public:
	~WebSocketManager();
	static WebSocketManager* instance();

	virtual void onOpen(cocos2d::extension::WebSocket* ws);
	virtual void onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data);
	virtual void onClose(cocos2d::extension::WebSocket* ws);
	virtual void onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error);

	//void setNet(PopoBirdSprite* popoBird){net=popoBird;};
	void sendMsg(std::string msg);
private:
	WebSocketManager();
	static WebSocketManager *m_instance;
	cocos2d::extension::WebSocket* m_wsiSendText;
	//PopoBirdSprite* net;
};

#endif // __WebSocketManager_H__

