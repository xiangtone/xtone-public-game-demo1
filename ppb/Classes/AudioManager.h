#ifndef __AudioManager_H__
#define __AudioManager_H__

static const char *soundeffect_candy = "candy.mp3";
static const char *soundeffect_click = "click.mp3";
static const char *soundeffect_dead = "dead.mp3";
static const char *soundeffect_hit = "hit.mp3";
static const char *soundeffect_jump = "jump.mp3";
static const char *soundeffect_swap = "swap.mp3";
static const char *soundeffect_shop = "shop.mp3";

class AudioManager
{
public:
	~AudioManager();
	static AudioManager* instance();

	void preLoadAudio();
	void playBGM(const char *filename);
	void stopBGM();
	void pauseBGM();
	void resumeBGM();
	unsigned int playEffect(const char *filename, bool isLoop = false);
	void stopEffect(unsigned int nSoundId);
	void setEffectVolume(float dt);
	void setBGMVolume(float dt);

private:
	AudioManager();
	static AudioManager *m_instance;
};

#endif //__AudioManager_H__