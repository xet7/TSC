#ifndef SMC_ERRORS_HPP
#define SMC_ERRORS_HPP
#include "global_game.hpp"
#include "../objects/movingsprite.hpp"

namespace SMC {

  /**
   * Base class of all SMC-related exceptions.
   */
  class SMCError: public std::exception
  {
  public:
    SMCError();
    virtual ~SMCError() throw();

    virtual const char* what() const throw();
  };

  /**
   * This exception is thrown when an expected XML
   * key is not found.
   */
  class XmlKeyDoesNotExist: public SMCError
  {
  public:
    XmlKeyDoesNotExist(std::string key);
    virtual ~XmlKeyDoesNotExist() throw();

    virtual const char* what() const throw();
    std::string Get_Key();
  protected:
    std::string m_key;
    };

	// Thrown if specific things are not implemented for
	// some reason.
	class NotImplementedError: public SMCError
	{
	public:
		NotImplementedError(std::string message);
		virtual ~NotImplementedError() throw();

		virtual const char* what() const throw();
	protected:
		std::string m_message;
	};

	class InvalidLevelError: public SMCError
	{
	public:
		InvalidLevelError(std::string message);
		virtual ~InvalidLevelError() throw();
		virtual const char* what() const throw();
	protected:
		std::string m_message;
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

}

#endif
