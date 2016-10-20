#include "Config.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <map>

const char* CONFIG_KEY_MUTE				= "mute";
const char* CONFIG_KEY_CHERRYNUM		= "cherryNum";
const char* CONFIG_KEY_GAMETIMES		= "gameTimes";
const char* CONFIG_KEY_PETTYPE			= "petType";
const char* CONFIG_KEY_HIGHSCORE		= "highScore";
const char* CONFIG_KEY_PETSAVEDSTRING   = "petSavedString";

using namespace cocos2d;

Config* Config::m_instance = NULL;

Config::Config()
: m_mute(false)
,m_savedCherryNum(0)
,m_savedGameTimes(0)
,m_savedPetType(0)
,m_savedHighScore(0)
{

}

Config::~Config()
{

}

Config* Config::instance()
{
	if (m_instance == NULL)
		m_instance = new Config;
	return m_instance;
}

void Config::load()
{
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	m_mute = ud->getBoolForKey(CONFIG_KEY_MUTE);
	m_savedCherryNum = ud->getIntegerForKey(CONFIG_KEY_CHERRYNUM);
	m_savedGameTimes = ud->getIntegerForKey(CONFIG_KEY_GAMETIMES);
	m_savedPetType = ud->getIntegerForKey(CONFIG_KEY_PETTYPE);
	m_savedHighScore = ud->getIntegerForKey(CONFIG_KEY_HIGHSCORE);
	m_savedPetString = ud->getStringForKey(CONFIG_KEY_PETSAVEDSTRING);

	if (m_savedPetString.empty()) {
		for (int i = 0; i < 5; i++) {
			m_savedPetString += '0';
		}
	}
}

void Config::flush()
{
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setBoolForKey(CONFIG_KEY_MUTE, m_mute);
	ud->setIntegerForKey(CONFIG_KEY_CHERRYNUM, m_savedCherryNum);
	ud->setIntegerForKey(CONFIG_KEY_GAMETIMES, m_savedGameTimes);
	ud->setIntegerForKey(CONFIG_KEY_PETTYPE, m_savedPetType);
	ud->setIntegerForKey(CONFIG_KEY_HIGHSCORE, m_savedHighScore);
	ud->setStringForKey(CONFIG_KEY_PETSAVEDSTRING, m_savedPetString);

	ud->flush();
}

void Config::setMute(bool mute)
{
	m_mute = mute;
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setBoolForKey(CONFIG_KEY_MUTE, m_mute);

	if (m_mute) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	}
	else {
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music.ogg", true);
	}
}

void Config::savePetString( std::string petSaves )
{
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	m_savedPetString.clear();
	m_savedPetString = petSaves;
	ud->setStringForKey(CONFIG_KEY_PETSAVEDSTRING, petSaves);
}

void Config::savePetType( int type )
{
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	m_savedPetType = type;
	ud->setIntegerForKey(CONFIG_KEY_PETTYPE, m_savedPetType);
}

void Config::addCherry( int num )
{
	m_savedCherryNum += num;

	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setIntegerForKey(CONFIG_KEY_CHERRYNUM, m_savedCherryNum);
}

void Config::useCherry(int num)
{
	m_savedCherryNum -= num;
	if (m_savedCherryNum < 0) {
		m_savedCherryNum = 0;
	}

	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setIntegerForKey(CONFIG_KEY_CHERRYNUM, m_savedCherryNum);
}

void Config::addGameTimes()
{
	m_savedGameTimes ++;

	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setIntegerForKey(CONFIG_KEY_GAMETIMES, m_savedGameTimes);
}

void Config::updateHighScore( int score )
{
	if (m_savedHighScore < score)
	{
		m_savedHighScore = score;

		CCUserDefault* ud = CCUserDefault::sharedUserDefault();
		ud->setIntegerForKey(CONFIG_KEY_HIGHSCORE, m_savedHighScore);
	}
}
