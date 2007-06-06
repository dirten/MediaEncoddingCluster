#ifndef ORG_ESB_LANG_NOSUCHELEMENTEXCEPTION_H_
#define ORG_ESB_LANG_NOSUCHELEMENTEXCEPTION_H_

#include <org/esb/lang/Exception.h>

namespace org{
    namespace esb{
	namespace lang{

    /*
     * Thrown from an operation that attempts to access some element that does
     * not exist.
     */
    class NoSuchElementException : public Exception
    {
    public:

        /**
         * Default Constructor
         */
        NoSuchElementException(void) {};

        /**
         * Conversion Constructor from some other ActiveMQException
         * @param An exception that should become this type of Exception
         */
        NoSuchElementException( const Exception& ex );

        /**
         * Copy Constructor
         */
        NoSuchElementException( const NoSuchElementException& ex );

        /**
         * Constructor - Initializes the file name and line number where
         * this message occured.  Sets the message to report, using an 
         * optional list of arguments to parse into the message
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param message to report
         * @param list of primitives that are formatted into the message
         */
        NoSuchElementException( const char* file, 
                                const int lineNumber,
                                const char* msg, ... );

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual Exception* clone(void) const;

        virtual ~NoSuchElementException(void)throw();

    };

}}}

#endif 
