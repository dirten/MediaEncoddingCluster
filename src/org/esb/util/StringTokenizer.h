#ifndef ORG_ESB_UTIL_STRINGTOKENIZER_H_
#define ORG_ESB_UTIL_STRINGTOKENIZER_H_

#include <org/esb/lang/Exception.h>
//#include <org/esb/lang/NoSuchElementException.h>
#include <string>
#include "exports.h"
using namespace org::esb::lang;
namespace org{
namespace esb{
namespace util{

    class UTIL_EXPORT StringTokenizer
    {
    private:
   
        // String to tokenize
        std::string str;
      
        // The delimiter string
        std::string delim;
      
        // The current pos in the string
        std::string::size_type pos;
      
        // Are we returning delimiters
        bool returnDelims;
      
    public:

        /**
         * Constructs a string tokenizer for the specified string. All 
         * characters in the delim argument are the delimiters for separating 
         * tokens.
         *
         * If the returnDelims flag is true, then the delimiter characters are 
         * also returned as tokens. Each delimiter is returned as a string of 
         * length one. If the flag is false, the delimiter characters are 
         * skipped and only serve as separators between tokens.
         *
         * Note that if delim is "", this constructor does not throw an 
         * exception. However, trying to invoke other methods on the resulting 
         * StringTokenizer may result in an Exception. 
         * @param str string to tokenize
         * @param delim String containing the delimiters
         * @param returnDelims boolean indicating if the delimiters are returned as tokens
         */
        StringTokenizer( const std::string& str,
                         const std::string& delim = " \t\n\r\f",
                         bool returnDelims = false);

        virtual ~StringTokenizer(void);
      
        /**
         * Calculates the number of times that this tokenizer's nextToken 
         * method can be called before it generates an exception. The current 
         * position is not advanced.
         * @return Count of remaining tokens
         */
        virtual int countTokens(void) const;

        /**
         * Tests if there are more tokens available from this tokenizer's 
         * string.
         * @return true if there are more tokens remaining
         */
        virtual bool hasMoreTokens(void) const;
      
        /**
         * Returns the next token from this string tokenizer.
         * @return string value of next token
         * @throws NoSuchElementException
         */
        virtual std::string nextToken(void) ;
//            throw ( Exception );
      
        /**
         * Returns the next token in this string tokenizer's string. First, 
         * the set of characters considered to be delimiters by this 
         * StringTokenizer object is changed to be the characters in the 
         * string delim. Then the next token in the string after the current 
         * position is returned. The current position is advanced beyond the 
         * recognized token. The new delimiter set remains the default after 
         * this call.
         * @param string containing the new set of delimiters
         * @return next string in the token list
         * @throw NoSuchElementException
         */
        virtual std::string nextToken(const std::string& delim) ;
//            throw ( Exception );

        /**
         * Grab all remaining tokens in the String and return them
         * in the vector that is passed in by reference.
         * @param vector to place token strings in
         * @return number of string placed into the vector
         */
        virtual unsigned int toArray( std::vector< std::string >& array );

        /**
         * Resets the Tokenizer's position in the String to the Beginning
         * calls to countToken and nextToken now start back at the beginning.
         * This allows this object to be reused, the caller need not create
         * a new instance every time a String needs tokenizing.
         * If set the string param will reset the string that this Tokenizer
         * is working on.  If set to "" no change is made.
         * If set the delim param will reset the string that this Tokenizer
         * is using to tokenizer the string.  If set to "", no change is made
         * If set the return Delims will set if this Tokenizer will return
         * delimiters as tokens. Defaults to false.
         * @param New String to tokenize or "", defaults to ""
         * @param New Delimiter String to use or "", defaults to ""
         * @param Should the Tokenizer return delimiters as Tokens, default false
         */
        virtual void reset( const std::string& str = "",
                            const std::string& delim = "",
                            bool returnDelims = false);
   
    };

}}}

#endif
