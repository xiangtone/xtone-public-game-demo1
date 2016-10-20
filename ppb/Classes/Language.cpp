#include "Language.h"

using namespace cocos2d;

Language* Language::m_instance = NULL;

Language::Language()
{

}

Language* Language::instance()
{
	if (!m_instance)
		m_instance = new Language;

	return m_instance;
}

void Language::load()
{
	m_stringDict = CCDictionary::createWithContentsOfFile("Language.plist");
	m_stringDict->retain();
}

const char* Language::getString( char *key )
{
	const char *s = NULL;
	if (m_stringDict) {
		CCString *str = (CCString *)m_stringDict->valueForKey(key);
		if (str) {
			s = str->getCString();
		}
	}

	return s;
}
