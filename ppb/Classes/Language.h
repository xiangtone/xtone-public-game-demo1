#ifndef __Language_H__
#define __Language_H__

#include "cocos2d.h"

#define	  GET_STRING(key)	(Language::instance()->getString(key))

class Language
{
public:
	Language();
	~Language() {};

	static Language* instance();
	void load();
	const char *getString(char *key);

private:
	static Language *m_instance;
	cocos2d::CCDictionary *m_stringDict;
};

#endif  //#ifndef __Language_H__