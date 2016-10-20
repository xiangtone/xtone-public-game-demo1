#include "GameScene.h"
#include "Contants.h"
#include "PopoBirdSprite.h"
#include "WallNode.h"
#include "HazardNode.h"
#include "CherryNode.h"
#include "BubbleNode.h"
#include "MainLayer.h"
#include "PopupLayer.h"
#include "GameOverLayer.h"
#include "Config.h"
#include "Language.h"
#include "AudioManager.h"
#include "AppuSDKInterfaceHelper.h"
#include "PopoBirdSprite.h"
#include "WebSocketManager.h"
using namespace std;

#define   RANDOM_IN_MIN_MAX(min, max) (rand() % (max-min) + min);

#define   MAIN_LAYER_TAG    (100)
#define	  CHERRY_NODE_TAG	(1000)
#define   BUBBLE_NODE_TAG	(1001)
#define   POPUP_LAYER_TAG   (200)

#define   DELAY_TIMES		(2)
#define   BUBBLE_TIMES		(1)

GameScene::GameScene()
{
	m_collisionNodes = CCArray::create();
	m_collisionNodes->retain();

	m_scores = 0;
	m_gameState = kGameStateNone;
	m_delayCount = DELAY_TIMES;
	m_bubbleTimes = 0;

	online=false;
}

GameScene::~GameScene()
{
	CC_SAFE_RELEASE(m_collisionNodes);

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUTTON_PRESS_ID);
}

CCScene* GameScene::scene()
{
    CCScene *pScene = CCScene::create();

	GameScene *layer = GameScene::create();
	pScene->addChild(layer);

    return pScene;
}

bool GameScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

	setTouchEnabled(true);
	
	initSpeets();
    m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    m_origin = CCDirector::sharedDirector()->getVisibleOrigin();

	createBackground();

	//create background
	m_backgroundNode = CCNode::create();
	m_backgroundNode->setAnchorPoint(CCPointZero);
	m_backgroundNode->setPosition(CCPointZero);
	m_backgroundNode->setContentSize(this->getContentSize());
	this->addChild(m_backgroundNode);
	addMainLayer();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
		callfuncO_selector(GameScene::notifyHandle), MSG_BUTTON_PRESS_ID, NULL);

// 	AppuSDKInterfaceHelper::instance()->init();
// 	AppuSDKInterfaceHelper::instance()->pay(100);
// 	AppuSDKInterfaceHelper::instance()->statistics(1, 6, "buy pet1");
// 	AppuSDKInterfaceHelper::instance()->exit();

    return true;
}

void GameScene::initSpeets()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pet.plist", "pet.png");
}

void GameScene::startGamePlaying()
{
	m_collisionNodes->removeAllObjects();
	m_collisionNodes->addObject(m_wallUpNode);
	m_collisionNodes->addObject(m_wallDownNode);

	//score clear
	m_scores = 0;

	//init popobird
	createPopoBird();
	//init hazard
	randomSetHazard(kHazardRight);
	//init cherry
	generateCherry();

	//start game loop
	this->scheduleUpdate();

	m_gameState = kGameStatePlaying;
	m_cheerys = 0;
	m_delayCount = DELAY_TIMES;
	m_bubbleTimes = 0;
	m_scoreLabel->setVisible(true);
	m_scoreLabel->setString((CCString::createWithFormat("%02d", m_scores))->getCString());
}

void GameScene::createBackground()
{
	float scalef = m_visibleSize.height/DESIGN_HEIGHT;

	CCSprite *bgSpt = CCSprite::create("bg.png");
	bgSpt->setAnchorPoint(ccp(0.5, 0.5));
	bgSpt->setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
	bgSpt->setScale(scalef);
	this->addChild(bgSpt, -1);

	m_bubbleTips = CCLabelTTF::create(
		CCString::createWithFormat(GET_STRING("BUBBLE_USE_TIPS"), 0)->getCString(), "Arial", 36);
	m_bubbleTips->setAnchorPoint(ccp(0.5, 1));
	m_bubbleTips->setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height - 150*scalef));
	m_bubbleTips->setColor(ccGRAY);
	m_bubbleTips->setVisible(false);
	this->addChild(m_bubbleTips, -1);

	//CCSprite *bgUpWallSpt = CCSprite::create("bg_hazard_up.png");
	WallNode *upWall = WallNode::create(kWallUp);
	upWall->setAnchorPoint(ccp(0.5, 1));
	upWall->setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height));
	this->addChild(upWall);
	m_wallUpNode = upWall;

	//CCSprite *bgDownWallSpt = CCSprite::create("bg_hazard_down.png");
	WallNode *downWall = WallNode::create(kWallDown);
	downWall->setAnchorPoint(ccp(0.5, 0));
	downWall->setPosition(ccp(m_visibleSize.width/2, 0));
	this->addChild(downWall);
	m_wallDownNode = downWall;

	//init score label
	m_scoreLabel = CCLabelBMFont::create("00", "numbers.fnt");
	m_scoreLabel->setAnchorPoint(ccp(0.5, 0.5));
	m_scoreLabel->setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
	m_scoreLabel->setColor(ccc3(156, 175, 173));
	m_scoreLabel->setOpacity(128);
	m_scoreLabel->setVisible(false);
	this->addChild(m_scoreLabel);
}

void GameScene::createPopoBird()
{
	m_popoBird = PopoBirdSprite::create();
	m_popoBird->setDelegate(this);
	m_popoBird->setAnchorPoint(ccp(0.5, 0.5));
	m_popoBird->setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
	m_backgroundNode->addChild(m_popoBird, 100);

	int petType = Config::instance()->petType();
	m_popoBird->setRole((PetType)petType);

	if(online){
		m_popoBirdNet = PopoBirdSprite::create();
		m_popoBirdNet->setDelegate(this);
		m_popoBirdNet->setAnchorPoint(ccp(0.5, 0.5));
		m_popoBirdNet->setPosition(ccp(m_visibleSize.width / 2, m_visibleSize.height / 2));
		m_backgroundNode->addChild(m_popoBirdNet, 100);
		m_popoBirdNet->setRole((PetType)1);
		//WebSocketManager::instance()->setNet(m_popoBirdNet);
	}
	
}

void GameScene::update( float delta )
{
	if (delta > 0.02f)
	{
		delta = 0.02f;
	}

	if (m_gameState != kGameStatePlaying)
	{
		return;
	}

	m_popoBird->updateWithDeltaTime(delta, m_collisionNodes);

	if(online){
		m_popoBirdNet->updateWithDeltaTime(delta, m_collisionNodes);
	}

}

void GameScene::generateCollisionObject()
{
	float probability = 0;

	probability = CCRANDOM_0_1();
	CCLog("%s%f","probability=",probability);
	if (probability < 0.6f && !m_popoBird->hasBubble() && m_delayCount <= 0)
	{
		generateBubble();
		m_delayCount = DELAY_TIMES;
	}
	else
	{
		generateCherry();
		m_delayCount --;
	}
}

void GameScene::generateBubble()
{
	BubbleNode *bubble = BubbleNode::create();
	bubble->setAnchorPoint(ccp(0.5, 0.5));
	bubble->setTag(BUBBLE_NODE_TAG);

	CCSize s = bubble->getContentSize();
	int randomX, randomY;
	getRandomPosition(s, randomX, randomY);
	bubble->setPosition(ccp(randomX, randomY));

	m_backgroundNode->addChild(bubble);
	m_collisionNodes->addObject(bubble);
}

void GameScene::generateHazard(int dirType, float y)
{
	HazardNode *hazardNode = HazardNode::create((HazardNodeType)dirType);
	if (dirType == kHazardLeft)
	{
		hazardNode->setAnchorPoint(ccp(0, 0));
		hazardNode->setPosition(ccp(0, y));
	}
	else
	{
		hazardNode->setAnchorPoint(ccp(1, 0));
		hazardNode->setPosition(ccp(m_visibleSize.width, y));
	}

	m_backgroundNode->addChild(hazardNode);
	m_collisionNodes->addObject(hazardNode);
}

void GameScene::randomSetHazard( int dirType )
{
	CCLog("randomSetHazard=%d", dirType);
	removeAllHazard();
	int hazardNum = 1;

	/*if (m_scores > 60)
	{
		hazardNum = 5;
	}
	else if (m_scores > 40)
	{
		hazardNum = 4;
	}
	else if (m_scores > 20)
	{
		hazardNum = 3;
	}
	else if (m_scores > 5)
	{
		hazardNum = 2;
	}*/
	if (m_scores > 0)
	{
		hazardNum = 10;
	}

	std::vector<int> yPosVec = randomYPos(hazardNum);

	for (unsigned int i = 0; i < yPosVec.size(); i++)
	{
		generateHazard(dirType, yPosVec[i]);
	}

	CCLog("randomSetHazard, count=%d", m_collisionNodes->count());
}

std::vector<int> GameScene::randomYPos( int num )
{
	std::vector<int> vecNums;

	int randomY[10] = {95, 195, 295, 395, 495, 595, 695, 795, 895, 995};

	std::vector<int> tempNums;
	for (int i = 0; i < 10; i++)
	{
		tempNums.push_back(randomY[i]);
	}

	for (int i = 0; i < num; i++)
	{
		int index = rand() % tempNums.size();
		vecNums.push_back(tempNums[index]);

		vector<int>::iterator it = tempNums.begin();
		for (; it != tempNums.end(); ++it)
		{
			if (*it == tempNums[index])
			{
				tempNums.erase(it);
				break;
			}
		}
	}

	return vecNums;
}

void GameScene::getRandomPosition(CCSize s, int &randomX, int &randomY)
{
	float distance;

	do
	{
		int min_x = s.width;
		int max_x = m_visibleSize.width - s.width;
		int min_y = s.height;
		int max_y = m_visibleSize.height - s.height;
		randomX = RANDOM_IN_MIN_MAX(min_x, max_x);
		randomY = RANDOM_IN_MIN_MAX(min_y, max_y);
		distance = ccpDistance(m_popoBird->getPosition(), ccp(randomX, randomY));
		if(online){
			distance = ccpDistance(m_popoBirdNet->getPosition(), ccp(randomX, randomY));
		}
		//log test
		CCLog("generateCherry =====distance=%2.2f======\n", distance);

	} while (distance < 200);
}

void GameScene::generateCherry()
{
	CherryNode *cherry = CherryNode::create();
	cherry->setAnchorPoint(ccp(0.5, 0.5));
	cherry->setTag(CHERRY_NODE_TAG);

	CCSize s = cherry->getContentSize();

	int randomX, randomY;
	getRandomPosition(s, randomX, randomY);

	cherry->setPosition(ccp(randomX, randomY));

	m_backgroundNode->addChild(cherry);
	m_collisionNodes->addObject(cherry);

	CCLog("generateCherry 11111");
}

void GameScene::updateScore()
{
	m_scoreLabel->setString((CCString::createWithFormat("%02d", ++m_scores))->getCString());
}

void GameScene::doAddCherryScoreAction( const CCPoint &pos )
{
	CCLabelBMFont *bmfont = CCLabelBMFont::create("+1", "language.fnt");
	bmfont->setAnchorPoint(ccp(0.5, 0.5));
	bmfont->setPosition(pos);
	m_backgroundNode->addChild(bmfont);

	CCActionInterval *moveBy = CCMoveBy::create(0.5f, ccp(0, 50));
	CCActionInterval *fadeOut = CCFadeOut::create(0.5f);
	CCSpawn *spawn = CCSpawn::create(moveBy, fadeOut, NULL);
	CCSequence *seq = CCSequence::create(spawn, CCRemoveSelf::create(), NULL);

	bmfont->runAction(seq);
}

void GameScene::addMainLayer()
{
	if (this->getChildByTag(MAIN_LAYER_TAG) != NULL) {
		return;
	}

	MainLayer *layer = MainLayer::create();
	layer->setTag(MAIN_LAYER_TAG);
	addChild(layer);

	m_scoreLabel->setVisible(false);
}

void GameScene::showPopupLayer( const char *tips )
{
	if (this->getChildByTag(POPUP_LAYER_TAG) != NULL) {
		return;
	}

	PopupLayer *popupLayer = PopupLayer::create();
	popupLayer->setTipString(tips);
	popupLayer->setTag(POPUP_LAYER_TAG);
	addChild(popupLayer);
}

void GameScene::showGameOverLayer()
{
	if (this->getChildByTag(300) != NULL) {
		return;
	}
	
	GameOverLayer *gameoverLayer = GameOverLayer::create();
	gameoverLayer->setTag(300);
	addChild(gameoverLayer);

	//saved scores
	Config::instance()->addCherry(m_cheerys);
	Config::instance()->addGameTimes();
	Config::instance()->updateHighScore(m_scores);

	int score = Config::instance()->highScore();
	int times = Config::instance()->gameTimes();
	gameoverLayer->setScore(m_scores);
	gameoverLayer->setCherrys(m_cheerys);
	gameoverLayer->updateGameText(score, times);
}

void GameScene::registerWithTouchDispatcher( void )
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (m_gameState != kGameStatePlaying)
	{
		return false;
	}

	m_popoBird->jump();

	if(online){
		//m_popoBirdNet->jump();
		WebSocketManager::instance()->sendMsg("jump");
	}
	return true;
}

void GameScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{

}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

}

void GameScene::didContact( CCNode *objectA, GameObject *objectB )
{
	if (objectB != NULL && objectB->getObjectType() == kWallType)
	{
		CCLog("wall Collision");
	}
	else if (objectB != NULL && objectB->getObjectType() == kHazardType)
	{
		CCLog("hazard Collision");
	}
	else if (objectB != NULL && objectB->getObjectType() == kCherryType)
	{
		CCLog("cherry Collision");
		doAddCherryScoreAction(objectB->getPosition());
		m_collisionNodes->removeObject(objectB);
		m_cheerys++;

		if (m_cheerys == 2 && !m_popoBird->hasBubble())
		{
			generateBubble();
			m_delayCount = DELAY_TIMES;
		}
		else
		{
			generateCollisionObject();
		}
	}
	else if (objectB != NULL && objectB->getObjectType() == kBubbleType)
	{
		CCLog("bubble Collision");
		m_collisionNodes->removeObject(objectB);

		generateCollisionObject();
	}
}

void GameScene::didGameOver()
{
	CCLog("Game Over");
	if (m_gameState == kGameStatePlaying)
	{
		m_gameState = KGameStateGameOver;
		//m_backgroundNode->removeAllChildren();
		m_backgroundNode->removeChildByTag(CHERRY_NODE_TAG);
		m_backgroundNode->removeChildByTag(BUBBLE_NODE_TAG);
		showGameOverLayer();
	}
}

void GameScene::turnDirectionLeft()
{
	CCLog("turn left");
	randomSetHazard(kHazardLeft);
	updateScore();
	CCLog("score=%d", m_scores);

	AudioManager::instance()->playEffect(soundeffect_hit);
}

void GameScene::turnDirectionRight()
{
	CCLog("turn right");
	randomSetHazard(kHazardRight);
	updateScore();
	CCLog("score=%d", m_scores);

	AudioManager::instance()->playEffect(soundeffect_hit);
}

void GameScene::didShowTips()
{
	if (m_bubbleTimes > 0) {
		m_bubbleTimes --;
		m_popoBird->addBubble();
		return;
	}

	if (this->getChildByTag(GAME_PLAY_POPUP_LAYER_TAG) != NULL) {
		return;
	}

	PopupLayer *popupLayer = PopupLayer::create();
	popupLayer->setTipString(GET_STRING("BUBBLE_BUY_TIPS"));
	popupLayer->setFeeTipsText(GET_STRING("BUBBLE_FEE_TIPS"));
	popupLayer->setConfirmBtnText(GET_STRING("BUBBLE_USE"));
	popupLayer->setParam(this);
	popupLayer->setTag(GAME_PLAY_POPUP_LAYER_TAG);
	this->addChild(popupLayer);

	m_gameState = kGameStatePause;

	this->setTouchEnabled(false);
}

void GameScene::removeAllHazard()
{
	//for (unsigned int i = 0; i < m_collisionNodes->count(); i++)
	if (m_collisionNodes->count() > 0)
	{
		for (int i = m_collisionNodes->count() - 1; i >= 0; --i)
		{
			GameObject *gameObject = dynamic_cast<GameObject *>(m_collisionNodes->objectAtIndex(i));
			if (gameObject != NULL && gameObject->getObjectType() == kHazardType)
			{
				m_collisionNodes->removeObject((CCNode *)gameObject);
				gameObject->removeFromParent();
			}
		}
	}

	CCLog("removeAllHazard, count=%d", m_collisionNodes->count());
}

void GameScene::notifyHandle(CCObject* obj)
{
	int id = ((CCInteger *)obj)->getValue();
	switch (id)
	{
	case MSG_ID_STARTLAYER_BUTTON_PRESS:
		{
			CCLog("MSG_ID_STARTLAYER_BUTTON_PRESS");

			CCNode *mainMenuLayer = this->getChildByTag(MAIN_LAYER_TAG);
			if (mainMenuLayer != NULL) {
				this->removeChild(mainMenuLayer);
			}

			startGamePlaying();

			AudioManager::instance()->playEffect(soundeffect_click);
			break;
		}
	case MSG_ID_GAVEOVERLAYER_BUTTON_PRESS:
		{
			CCLog("MSG_ID_GAVEOVERLAYER_BUTTON_PRESS");

			CCNode *gameOverLayer = this->getChildByTag(300);
			if (gameOverLayer != NULL) {
				this->removeChild(gameOverLayer);
			}

			m_backgroundNode->removeAllChildren();
			addMainLayer();

			AudioManager::instance()->playEffect(soundeffect_click);
			break;
		}
	case MSG_ID_POPUPLAYER_BUTTON_PRESS:
		{
			CCLog("MSG_ID_POPUPLAYER_BUTTON_PRESS");

			CCNode *mainMenuLayer = this->getChildByTag(MAIN_LAYER_TAG);
			if (mainMenuLayer != NULL) {
				this->removeChild(mainMenuLayer);
			}

			startGamePlaying();

			AudioManager::instance()->playEffect(soundeffect_shop);

			break;
		}
	case MSG_ID_POPUPLAYER_BACKTOGAME:
		{
			m_gameState = kGameStatePlaying;
			break;
		}
	case MSG_ID_POPUPLAYER_GAMEPLAYING_BUTTON_PRESS:
		{
			m_bubbleTimes = BUBBLE_TIMES;
			m_popoBird->addBubble();
			AudioManager::instance()->playEffect(soundeffect_shop);

			break;
		}
	case MSG_ID_POPUPLAYER_JION:
		{
			CCLog("%s","game notify login");
			online=true;
			break;
		}
	case MSG_ID_POPUPLAYER_JUMP:
		{
			CCLog("%s","game notify jump");
			if(online){
				m_popoBirdNet->jump();
			}
			break;
		}

	default:
		break;
	}
}

void GameScene::updateBubbleSecondLabel(int second)
{
	m_bubbleTips->setString(
		CCString::createWithFormat(GET_STRING("BUBBLE_USE_TIPS"), second)->getCString());
}

void GameScene::updateBubbleSecond(int sec)
{
	if (sec == BIRD_BUBBLE_DELAY_TIME) {
		m_bubbleTips->setVisible(true);
	} else if (sec <= 0) {
		m_bubbleTips->setVisible(false);
	}

	updateBubbleSecondLabel(sec);
}
