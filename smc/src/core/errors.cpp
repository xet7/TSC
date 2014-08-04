#include "errors.hpp"

using namespace SMC;
using namespace std;

SMCError::SMCError()
{
	m_smc_errmsg = "Unknown SMC exception.";
}


SMCError::SMCError(string message)
{
	m_smc_errmsg = message;
}

SMCError::~SMCError() throw()
{
	//
}

const char* SMCError::what() const throw()
{
	return m_smc_errmsg.c_str();
}

ConfigurationError::ConfigurationError(string msg)
	: SMCError(msg)
{
	//
}

ConfigurationError::~ConfigurationError() throw()
{
	//
}

XmlKeyDoesNotExist::XmlKeyDoesNotExist(string key)
{
	m_key = key;
}

XmlKeyDoesNotExist::~XmlKeyDoesNotExist() throw()
{
	//
}

string XmlKeyDoesNotExist::Get_Key()
{
	return m_key;
}

const char* XmlKeyDoesNotExist::what() const throw()
{
    string err = "XML key '" + m_key + "' does not exist!\n";
	return err.c_str();
}

NotImplementedError::NotImplementedError(string message)
	: SMCError(message)
{
}

NotImplementedError::~NotImplementedError() throw()
{
	//
}

InvalidLevelError::InvalidLevelError(string message)
	: SMCError(message)
{
	//
}

InvalidLevelError::~InvalidLevelError() throw()
{
	//
}

RestartedXmlParserError::RestartedXmlParserError()
{
	//
}

RestartedXmlParserError::~RestartedXmlParserError() throw()
{
	//
}

const char* RestartedXmlParserError::what() const throw()
{
	return "Restarted a one-time XML parser!";
}

InvalidMovingStateError::InvalidMovingStateError(Moving_state state)
{
	m_state = state;
}

InvalidMovingStateError::~InvalidMovingStateError() throw()
{
	//
}

const char* InvalidMovingStateError::what() const throw()
{
    stringstream ss;
	ss << "Invalid moving state '" << m_state << "' for this object!\n";	
	return ss.str().c_str();
}

EditorError::EditorError(string msg)
	: SMCError(msg)
{
	//
}

EditorError::~EditorError() throw()
{
	//
}

EditorSpriteCopyFailedError::EditorSpriteCopyFailedError(cSprite* p_sprite)
    : EditorError(string("Editor sprite '") + p_sprite->Create_Name() + "' copy failed!")
{
	mp_sprite = p_sprite;
}

EditorSpriteCopyFailedError::~EditorSpriteCopyFailedError() throw()
{
	//
}

cSprite* EditorSpriteCopyFailedError::Get_Sprite()
{
	return mp_sprite;
}
