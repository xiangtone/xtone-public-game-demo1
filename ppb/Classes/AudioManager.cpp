#include "AudioManager.h"
#include "SimpleAudioEngine.h"
#include "Config.h"

using namespace CocosDenshion;

AudioManager* AudioManager::m_instance = NULL;

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

AudioManager* AudioManager::instance()
{
	if (m_instance == NULL)
		m_instance = new AudioManager();
	return m_instance;
}

void AudioManager::preLoadAudio()
{
	//¼ÓÔØÒôÀÖ
	////¼ÓÔØÒôÐ§
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_candy);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_click);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_dead);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_hit);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_jump);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_swap);
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundeffect_shop);
}

void AudioManager::playBGM(const char *filename)
{
	if (Config::instance()->mute())
		return;

	stopBGM();

	if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename, true);
	}
}

void AudioManager::stopBGM()
{
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
}

void AudioManager::pauseBGM()
{
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioManager::resumeBGM()
{
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

unsigned int AudioManager::playEffect(const char *filename, bool isLoop)
{
	if (Config::instance()->mute())
	{
		return 0;
	}

	return SimpleAudioEngine::sharedEngine()->playEffect(filename, isLoop);
}

void AudioManager::stopEffect( unsigned int nSoundId )
{
	SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void AudioManager::setEffectVolume( float dt )
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(SimpleAudioEngine::sharedEngine()->getEffectsVolume()+dt);
}

void AudioManager::setBGMVolume( float dt )
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume()+dt);
}
