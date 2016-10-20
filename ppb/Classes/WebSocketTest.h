#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/WebSocket.h"

class WebSocketTestLayer
: public cocos2d::CCLayer
, public cocos2d::extension::WebSocket::Delegate
{
public: 
    virtual void onOpen(cocos2d::extension::WebSocket* ws);
    virtual void onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data);
    virtual void onClose(cocos2d::extension::WebSocket* ws);
    virtual void onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error);
private:
    cocos2d::extension::WebSocket* _wsiSendText;
    cocos2d::extension::WebSocket* _wsiSendBinary;
    cocos2d::extension::WebSocket* _wsiError;
    
    int _sendTextTimes;
    int _sendBinaryTimes;
};

void runWebSocketTest();

