//
//  WebSocketTest.cpp
//  TestCpp
//
//  Created by James Chen on 5/31/13.
//
//

#include "WebSocketTest.h"

USING_NS_CC;
USING_NS_CC_EXT;

WebSocketTestLayer::WebSocketTestLayer()
: _wsiSendText(NULL)
, _wsiSendBinary(NULL)
, _wsiError(NULL)
, _sendTextTimes(0)
, _sendBinaryTimes(0)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    _wsiSendText = new WebSocket();
    _wsiSendBinary = new WebSocket();
    _wsiError = new WebSocket();
    
    if (!_wsiSendText->init(*this, "ws://echo.websocket.org"))
    {
        CC_SAFE_DELETE(_wsiSendText);
    }
    
    if (!_wsiSendBinary->init(*this, "ws://echo.websocket.org"))
    {
        CC_SAFE_DELETE(_wsiSendBinary);
    }
    
    if (!_wsiError->init(*this, "ws://invalid.url.com"))
    {
        CC_SAFE_DELETE(_wsiError);
    }
	/*if (!_wsiSendText->init(*this, "ws://127.0.0.1:8080/"))
    {
        CC_SAFE_DELETE(_wsiSendText);
    }
    
    if (!_wsiSendBinary->init(*this, "ws://127.0.0.1:8080/"))
    {
        CC_SAFE_DELETE(_wsiSendBinary);
    }
    
    if (!_wsiError->init(*this, "ws://127.0.0.1:8080/"))
    {
        CC_SAFE_DELETE(_wsiError);
    }*/
	
}


WebSocketTestLayer::~WebSocketTestLayer()
{
    if (_wsiSendText)
        _wsiSendText->close();
    
    if (_wsiSendBinary)
        _wsiSendBinary->close();
    
    if (_wsiError)
        _wsiError->close();
}

// Delegate methods
void WebSocketTestLayer::onOpen(cocos2d::extension::WebSocket* ws)
{
    CCLog("Websocket (%p) opened", ws);
    if (ws == _wsiSendText)
    {
        CCLog("%s","Send Text WS was opened.");
    }
    else if (ws == _wsiSendBinary)
    {
        CCLog("%s","Send Binary WS was opened.");
    }
    else if (ws == _wsiError)
    {
        CCAssert(0, "error test will never go here.");
    }
}

void WebSocketTestLayer::onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data)
{
    if (!data.isBinary)
    {
        _sendTextTimes++;
        char times[100] = {0};
        sprintf(times, "%d", _sendTextTimes);
        std::string textStr = std::string("response text msg: ")+data.bytes+", "+times;
        CCLog("%s", textStr.c_str());
    }
    else
    {
        _sendBinaryTimes++;
        char times[100] = {0};
        sprintf(times, "%d", _sendBinaryTimes);

        std::string binaryStr = "response bin msg: ";
        
        for (int i = 0; i < data.len; ++i) {
            if (data.bytes[i] != '\0')
            {
                binaryStr += data.bytes[i];
            }
            else
            {
                binaryStr += "\'\\0\'";
            }
        }
        
        binaryStr += std::string(", ")+times;
        CCLog("%s", binaryStr.c_str());
    }
}

void WebSocketTestLayer::onClose(cocos2d::extension::WebSocket* ws)
{
    CCLog("websocket instance (%p) closed.", ws);
    if (ws == _wsiSendText)
    {
        _wsiSendText = NULL;
    }
    else if (ws == _wsiSendBinary)
    {
        _wsiSendBinary = NULL;
    }
    else if (ws == _wsiError)
    {
        _wsiError = NULL;
    }
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void WebSocketTestLayer::onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error)
{
    CCLog("Error was fired, error code: %d", error);
    if (ws == _wsiError)
    {
        char buf[100] = {0};
        sprintf(buf, "an error was fired, code: %d", error);
    }
}

void runWebSocketTest()
{
    CCScene *pScene = CCScene::create();
    WebSocketTestLayer *pLayer = new WebSocketTestLayer();
    pScene->addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
    pLayer->release();
}
