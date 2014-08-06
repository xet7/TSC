#ifndef SMC_ERRORS_HPP
#define SMC_ERRORS_HPP
#include "global_game.hpp"
#include "../objects/movingsprite.hpp"

using namespace std;

namespace SMC {

  /**
   * Base class of all SMC-related exceptions.
   */
  class SMCError: public std::exception
  {
  public:
    SMCError();
    SMCError(string message);
    virtual ~SMCError() throw();

    virtual const char* what() const throw();
  protected:
      string m_smc_errmsg;
  };

  class ConfigurationError: public SMCError
  {
  public:
      ConfigurationError(string message);
	  virtual ~ConfigurationError() throw();
  };

  /**
   * This exception is thrown when an expected XML
   * key is not found.
   */
  class XmlKeyDoesNotExist: public SMCError
  {
  public:
    XmlKeyDoesNotExist(string key);
    virtual ~XmlKeyDoesNotExist() throw();

    virtual const char* what() const throw();
    string Get_Key();
  protected:
    string m_key;
    };

	// Thrown if specific things are not implemented for
	// some reason.
	class NotImplementedError: public SMCError
	{
	public:
        NotImplementedError(string message);
		virtual ~NotImplementedError() throw();
	};

	class InvalidLevelError: public SMCError
	{
	public:
        InvalidLevelError(string message);
		virtual ~InvalidLevelError() throw();
	};

	class RestartedXmlParserError: public SMCError
	{
	public:
		RestartedXmlParserError();
		virtual ~RestartedXmlParserError() throw();
		virtual const char* what() const throw();
	};

	class InvalidMovingStateError: public SMCError
	{
	public:
		InvalidMovingStateError(Moving_state state);
		virtual ~InvalidMovingStateError() throw();
		virtual const char* what() const throw();
	protected:
		Moving_state m_state;
	};

	class EditorError: public SMCError
	{
	public:
        EditorError(string msg);
		virtual ~EditorError() throw();
	};

	class EditorSpriteCopyFailedError: public EditorError
	{
	public:
		EditorSpriteCopyFailedError(cSprite* p_sprite);
		virtual ~EditorSpriteCopyFailedError() throw();
		cSprite* Get_Sprite();
	protected:
		cSprite* mp_sprite;
	};

}

#endif
