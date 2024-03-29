#ifndef ORG_ESB_LANG_EXCEPTION_H
#define ORG_ESB_LANG_EXCEPTION_H

#include "org/esb/lang/Throwable.h"
#include <stdarg.h>
#include <sstream>
#include "exports.h"

#define BACKTRACE
namespace org {
  namespace esb {
    namespace lang {

      /**
       * Base class for all exceptions.
       */
      class LANG_EXPORT Exception : public Throwable {
      private:

        /**
         * The cause of this exception.
         */
        std::string message;

        /**
         * The stack trace.
         */
        std::vector< std::pair< std::string, int> > stackTrace;

        std::string _msg;
      public:

        /**
         * Default Constructor
         */
        Exception(void) {
        }

        /**
         * Copy Constructor
         */
        Exception(const Exception& ex) {
          *this = ex;
        }

        Exception(const char * msg):_msg(msg) {
          message=_msg;
        }

        Exception(const std::string msg):_msg(msg) {
          message=_msg;
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occured.  Sets the message to report, using an 
         * optional list of arguments to parse into the message
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param message to report
         * @param list of primitives that are formatted into the message
         */
        Exception(const char* file, const int lineNumber,
            const std::string msg, ...):_msg(msg) {
          va_list vargs;
          va_start(vargs, msg);
          buildMessage(_msg, vargs);

          // Set the first mark for this exception.
          setMark(file, lineNumber);
        }

        virtual ~Exception()throw () {
        }

        /**
         * Gets the message for this exception.
         * @return Text formatted error message
         */
        virtual std::string getMessage() const {
          return message;
        }

        /**
         * Sets the cause for this exception.
         * @param msg the format string for the msg.
         * @param variable - params to format into the string
         */
		virtual void setMessage(const std::string msg, ...) {
          va_list vargs;
          va_start(vargs, msg);
          buildMessage(msg, vargs);
        }

        /**
         * Adds a file/line number to the stack trace.
         * @param file The name of the file calling this method (use __FILE__).
         * @param lineNumber The line number in the calling file (use __LINE__).
         */
        virtual void setMark(const char* file, const int lineNumber);

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         * @return Copy of this Exception object
         */
        virtual Exception* clone() const {
          return new Exception(*this);
        }

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.  The first
         * item in the returned vector is the first point where the mark
         * was set (e.g. where the exception was created).  
         * @return the stack trace.
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const {
          return stackTrace;
        }

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const {
          printStackTrace(std::cerr);
        }

        /**
         * Prints the stack trace to the given output stream.
         * @param stream the target output stream.
         */
        virtual void printStackTrace(std::ostream& stream) const {
          stream << getStackTraceString();
        }

        /**
         * Gets the stack trace as one contiguous string.
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const {

          // Create the output stream.
          std::ostringstream stream;

          // Write the message and each stack entry.
          stream << message << std::endl;
          for (unsigned int ix = 0; ix < stackTrace.size(); ++ix) {
            stream << "\tFILE: " << stackTrace[ix].first;
            stream << ", LINE: " << stackTrace[ix].second;
            stream << std::endl;
          }

          // Return the string from the output stream.
          return stream.str();
        }

        /**
         * Assignment operator.
         * @param const reference to another ActiveMQException
         */
        virtual Exception & operator =(const Exception& ex) {
          this->message = ex.message;
          this->stackTrace = ex.stackTrace;
          return *this;
        }



        void buildMessage(const std::string format, va_list& vargs);
#ifdef BACKTRACE
        virtual const char * what()const throw ();
#endif
      };

    }
  }
}

#endif
