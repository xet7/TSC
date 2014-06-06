#include "errors.hpp"

using namespace SMC;

SMCError::SMCError()
{
	//
}

SMCError::~SMCError() throw()
{
	//
}

const char* SMCError::what() const throw()
{
	return "Unknown SMC exception.\n";
}

ConfigurationError::ConfigurationError(std::string msg)
{
	m_message = msg;
}

ConfigurationError::~ConfigurationError() throw()
{
	//
}

const char* ConfigurationError::what() const throw()
{
	return m_message.c_str();
}

XmlKeyDoesNotExist::XmlKeyDoesNotExist(std::string key)
{
	m_key = key;
}

XmlKeyDoesNotExist::~XmlKeyDoesNotExist() throw()
{
	//
}

std::string XmlKeyDoesNotExist::Get_Key()
{
	return m_key;
}

const char* XmlKeyDoesNotExist::what() const throw()
{
	std::string err = "XML key '" + m_key + "' does not exist!\n";
	return err.c_str();
}

NotImplementedError::NotImplementedError(std::string message)
{
	m_message = message;
}

NotImplementedError::~NotImplementedError() throw()
{
	//
}

const char* NotImplementedError::what() const throw()
{
	return m_message.c_str();
}

InvalidLevelError::InvalidLevelError(std::string message)
{
	m_message = message;
}

InvalidLevelError::~InvalidLevelError() throw()
{
	//
}

const char* InvalidLevelError::what() const throw()
{
	return m_message.c_str();
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
	std::stringstream ss;
	ss << "Invalid moving state '" << m_state << "' for this object!\n";	
	return ss.str().c_str();
}

EditorError::EditorError(std::string msg)
{
	m_msg = msg;
}

EditorError::~EditorError() throw()
{
	//
}

const char* EditorError::what() const throw()
{
	return m_msg.c_str();
}

EditorSpriteCopyFailedError::EditorSpriteCopyFailedError(cSprite* p_sprite)
	: EditorError(std::string("Editor sprite '") + p_sprite->Create_Name() + "' copy failed!")
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
