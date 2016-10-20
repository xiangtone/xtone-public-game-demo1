#include "PopoBirdSprite.h"
#include "GameObject.h"
#include "AudioManager.h"

#define  BIRD_PARTICLE_TAG	     (100)
#define  BIRD_BUBBLE_TAG         (101)

#define  BIRD_SPEED_X			(330)
#define  BIRD_GRAVITY_Y			(-1800)
#define  BIRD_JUMP_FORCE		(650)

PopoBirdSprite::PopoBirdSprite()
{
	m_delegate = NULL;
	m_state = kPopoBirdStateNone;

	m_velocity = ccp(BIRD_SPEED_X, 0);
	m_gravity = ccp(0, BIRD_GRAVITY_Y);

	m_hasBubble = false;
	m_jumpSoundId = 0;

	/*life=0;
	CCLog("%s%d","left=",life);*/
}

PopoBirdSprite::~PopoBirdSprite()
{
	m_delegate = NULL;
	CC_SAFE_RELEASE(m_flyAnimation);
}

bool PopoBirdSprite::init()
{
	bool bRet = false;
	do 
	{
		//bRet = CCSprite::initWithFile("pet_01.png");
		bRet = CCSprite::initWithSpriteFrameName("pet01_01.png");
		if (!bRet)
		{
			break;
		}

		CCSize s = CCDirector::sharedDirector()->getVisibleSize();
		m_screenWidth = s.width;
		m_screenHeight = s.height;

		m_state = kPopoBirdStateRightMoving;

		bRet = true;
	} while(false);

	return bRet;
}

void PopoBirdSprite::onEnter()
{
	CCSprite::onEnter();

	addParticle();
}

void PopoBirdSprite::updateWithDeltaTime( float dt, CCArray *listObjects )
{
	if (this->getPositionX()-this->getContentSize().width/2 < 0)
	{
		CCLog("left getPositionX()=%2.2f", this->getPositionX());

		if (m_delegate && m_state == kPopoBirdStateLeftMoving)
		{
			this->setPositionX(this->getContentSize().width/2);

			m_delegate->turnDirectionRight();
			m_state = kPopoBirdStateRightMoving;
			m_velocity.x = -m_velocity.x;
			this->setFlipX(false);
		}

		if (m_state == kPopoBirdStateDead)
		{
			m_velocity.x = -m_velocity.x;
		}
	}

	if (this->getPositionX()+this->getContentSize().width/2 > m_screenWidth)
	{
		CCLog("right getPositionX()=%2.2f", this->getPositionX());

		if (m_delegate && m_state == kPopoBirdStateRightMoving)
		{
			this->setPositionX(m_screenWidth - this->getContentSize().width/2);

			m_delegate->turnDirectionLeft();
			m_state = kPopoBirdStateLeftMoving;
			m_velocity.x = -m_velocity.x;
			this->setFlipX(true);
		}

		if (m_state == kPopoBirdStateDead)
		{
			m_velocity.x = -m_velocity.x;
		}
	}

	if (m_state == kPopoBirdStateDead)
	{
		if (this->getPositionY() - this->getContentSize().height/2 < 0)
		{
			m_velocity.y = -m_velocity.y;
		}

		if (this->getPositionY() + this->getContentSize().height/2 > m_screenHeight)
		{
			m_velocity.y = -m_velocity.y;
		}
	}
	else
	{
		this->checkCollisions(listObjects);
		m_velocity.y = m_velocity.y + m_gravity.y * dt;
	}

	//bubble down edge jump
	if (m_hasBubble && this->getPositionY() - this->getContentSize().height/2 < 0)
	{
		m_velocity.y = 600;
	}

	CCPoint amtPoint = m_velocity * dt;
	const CCPoint newPoint = this->getPosition() + amtPoint;
	this->setPosition(newPoint);
}

void PopoBirdSprite::jump()
{
	if (m_state == kPopoBirdStateDead)
	{
		return;
	}

	CCPoint jumpforce = ccp(0, BIRD_JUMP_FORCE);
	m_velocity.y = jumpforce.y;

	this->playFlyAnimation();

	AudioManager::instance()->stopEffect(m_jumpSoundId);
	m_jumpSoundId = AudioManager::instance()->playEffect(soundeffect_jump);
}

void PopoBirdSprite::push()
{
	CCPoint pushforce = ccp(1000, 1000);
	m_velocity = pushforce;
}

void PopoBirdSprite::addParticle()
{
	CCParticleSystemQuad *emitter = new CCParticleSystemQuad();

	std::string filename = "point.plist";
	emitter->initWithFile(filename.c_str());
	emitter->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
	emitter->setTag(BIRD_PARTICLE_TAG);
	
	this->addChild(emitter, -1);
}

void PopoBirdSprite::loadAnimation(PetType type)
{
	char str[64] = {0};

	//create every frame to array
	CCArray *animFrames = CCArray::createWithCapacity(2);
	for (int i = 1; i <= 2; i++)
	{
		memset(str, 0, sizeof(str));
		sprintf(str, "%s_%02d.png", s_pet_list[type], i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	m_flyAnimation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
	m_flyAnimation->retain();
}

void PopoBirdSprite::animateOverHandler()
{
	CCString *str = CCString::createWithFormat("%s_01.png", s_pet_list[m_roleType]);
	CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());
	if (spriteFrame != NULL)
	{
		this->setDisplayFrame(spriteFrame);
	}
}

void PopoBirdSprite::playFlyAnimation()
{
	this->stopAllActions();

	CCAnimate *animate = CCAnimate::create(m_flyAnimation);
	CCFiniteTimeAction *timeoverHdlr = CCCallFunc::create(this, callfunc_selector(PopoBirdSprite::animateOverHandler));
	CCSequence *seq = CCSequence::create(animate, timeoverHdlr, NULL);

	this->runAction(seq);
}

void PopoBirdSprite::addBubble()
{
	m_hasBubble = true;
	m_bubbleTimeCnt = BIRD_BUBBLE_DELAY_TIME;
	this->schedule(schedule_selector(PopoBirdSprite::bubbleTimeHandler), 1.f);
	if (m_delegate) {
		m_delegate->updateBubbleSecond(BIRD_BUBBLE_DELAY_TIME);
	}

	CCNode *bubbleNode = this->getChildByTag(BIRD_BUBBLE_TAG);
	if (bubbleNode == NULL) {
		CCSprite *bubble = CCSprite::create("bubble.png");
		bubble->setAnchorPoint(ccp(0.5, 0.5));
		bubble->setTag(BIRD_BUBBLE_TAG);
		bubble->setPosition(
			ccp(this->getContentSize().width/2, this->getContentSize().height/2));
		this->addChild(bubble);
	}
}

void PopoBirdSprite::bubbleTimeHandler(float dt)
{
	if (m_bubbleTimeCnt > 0) {
		if (m_delegate) {
			m_delegate->updateBubbleSecond(m_bubbleTimeCnt);
		}

		m_bubbleTimeCnt --;
	} else {
		m_hasBubble = false;

		CCNode *node = this->getChildByTag(BIRD_BUBBLE_TAG);
		if (node != NULL) {
			node->setVisible(false);
			node->removeFromParent();
		}

		if (m_delegate) {
			m_delegate->updateBubbleSecond(m_bubbleTimeCnt);
		}

		this->unschedule(schedule_selector(PopoBirdSprite::bubbleTimeHandler));
	}
}

bool PopoBirdSprite::collisionWithOther( CCNode *other )
{
	bool bRet = false;
	GameObject *collider = (GameObject *)other;

	if (!collider)
		return false;

	switch (collider->getObjectType())
	{
	case kHazardType:
		{
			//CCLog("hazard collision!");
			if (!m_hasBubble)
			{
				dead();

				bRet = true;
				break;
			}
		}
	case kCherryType:
		{
			AudioManager::instance()->playEffect(soundeffect_candy);
			bRet = true;
			break;
		}
	case kWallType:
		{
			if (!m_hasBubble)
			{
				//CCLog("wall collision!");
				dead();

				bRet = true;
				break;
			}
		}
	case kBubbleType:
		{
			if (!m_hasBubble)
			{
				//showTips();
				addBubble();
			}

			bRet = true;
			break;
		}
	default:
		break;
	}

	return bRet;
}

void PopoBirdSprite::checkCollisions(CCArray *listObjects)
{
	if (listObjects->count() <= 0)
	{
		return;
	}

	for (int i = listObjects->count() - 1; i >= 0 ; --i)
	{
		GameObject *gameObject = dynamic_cast<GameObject *>(listObjects->objectAtIndex(i));
		if (gameObject != NULL)
		{
			CCRect rect = gameObject->adjustedBoundingBox();
			CCRect playerRect = this->adjustedBoundingBox();

			if (playerRect.intersectsRect(rect))
			{
				bool bRet = false;

				bRet = this->collisionWithOther(gameObject);
				if (bRet)
				{
					bRet = gameObject->collisionWithOther(this);
					if (bRet && m_delegate != NULL)
					{
						m_delegate->didContact(this, gameObject);
					}
				}
			}
		}
	}
}

void PopoBirdSprite::deadActionCallback()
{
	if (m_delegate)
	{
		m_delegate->didGameOver();
	}
}


void PopoBirdSprite::dead()
{
	//life--;
	m_state = kPopoBirdStateDead;
	push();

	stopAllActions();
	removeChildByTag(BIRD_PARTICLE_TAG);

	CCSprite *deadFrame = CCSprite::create("pet_dead.png");
	this->setDisplayFrame(deadFrame->displayFrame());

	//if(life==0){
	CCRotateBy *rotateBy = CCRotateBy::create(0.5f, 360);
	CCRepeat *repeat = CCRepeat::create(rotateBy, 4);
	CCFadeOut *fadeOut = CCFadeOut::create(2.f);
	CCSpawn *spawn = CCSpawn::create(repeat, fadeOut, NULL);
	CCSequence *seq = CCSequence::create(spawn, CCCallFunc::create(
		this, callfunc_selector(PopoBirdSprite::deadActionCallback)), NULL);

	this->runAction(seq);
	/*}else{
	CCRotateBy *rotateBy = CCRotateBy::create(0.5f, 360);
	CCRepeat *repeat = CCRepeat::create(rotateBy, 4);
	CCFadeOut *fadeOut = CCFadeOut::create(2.f);
	CCSpawn *spawn = CCSpawn::create(repeat, fadeOut, NULL);
	CCSequence *seq = CCSequence::create(spawn, NULL, NULL);
	this->runAction(seq);
	}*/

	AudioManager::instance()->playEffect(soundeffect_dead);
}

void PopoBirdSprite::setRole( PetType type )
{
	CCString *str = CCString::createWithFormat("%s_01.png", s_pet_list[type]);
	CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());
	if (spriteFrame != NULL)
	{
		this->setDisplayFrame(spriteFrame);
	}

	loadAnimation(type);

	CCParticleSystemQuad *emitter = (CCParticleSystemQuad *)this->getChildByTag(BIRD_PARTICLE_TAG);
	if (emitter)
	{
		emitter->setStartColor(ccc4FFromccc3B(s_particle_color[type]));
	}

	m_roleType = type;
}

void PopoBirdSprite::showTips()
{
	if (m_delegate)
	{
		m_delegate->didShowTips();
	}
}

//void PopoBirdSprite::addLife(){
//	life++;
//}
cocos2d::CCRect PopoBirdSprite::adjustedBoundingBox()
{
	CCRect r;
	r = this->boundingBox();

	float xOffset = r.size.width * 0.1f;
	float yOffset = r.size.height * 0.1f;
	float xCropAmount = r.size.width * 0.25f;
	float yCropAmount = r.size.height * 0.25f;

	r = CCRectMake(r.origin.x + xOffset, 
				   r.origin.y + yOffset, 
				   r.size.width - xCropAmount,
				   r.size.height - yCropAmount);

	return r;
}

#ifdef __DEBUG_DRAW__
void PopoBirdSprite::draw()
{
	CCSprite::draw();

	ccDrawColor4B(0, 255, 255, 255);

	CCRect r = this->adjustedBoundingBox();

	float xOffset, yOffset;
	xOffset = r.size.width * 0.1f;
	yOffset = r.size.height * 0.1f;

	const CCPoint &p1 = ccp(xOffset, yOffset);
	const CCPoint &p2 = ccp(p1.x + r.size.width, p1.y + r.size.height);
	ccDrawRect( p1, p2 );

	CHECK_GL_ERROR_DEBUG();
}

#endif  //#ifdef __DEBUG_DRAW__
