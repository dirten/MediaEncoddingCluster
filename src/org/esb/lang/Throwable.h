#ifndef ORG_ESB_LANG_THROWABLE_H
#define ORG_ESB_LANG_THROWABLE_H
 
// Includes
#include <string>
#include <vector>
#include <iostream>

namespace org{
namespace esb{
namespace lang{
    
    /**
     * This class represents an error that has occurred in 
     * cms.
     */
    class Throwable{
        
    public:
        
        virtual ~Throwable(){}
        
        /**
         * Gets the cause of the error.
         * @return string errors message
         */
        virtual std::string getMessage() const = 0;
        
        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.
         * @return vector containing stack trace strings
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const = 0;
        
        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const = 0;
        
        /**
         * Prints the stack trace to the given output stream.
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const = 0;
        
    };

}}}

#endif
