#ifndef ORG_ESB_UTIL_PROPERTIES_H_
#define ORG_ESB_UTIL_PROPERTIES_H_
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include <map>
#include <string>
#include <vector>
#include "exports.h"
namespace org{
namespace esb{
namespace util{
   
    /**
     * Interface for a Java-like properties object.  This is essentially 
     * a map of key-value string pairs.
     */
    class UTIL_EXPORT Properties{
    private:
	std::map< std::string, std::string > properties;

    public:
   
        virtual ~Properties();
        
        /**
         * Returns true if the properties object is empty
         * @return true if empty
         */
        virtual bool isEmpty() const;
        
        /**
         * Looks up the value for the given property.
         * @param name The name of the property to be looked up.
         * @return the value of the property with the given name, if it
         * exists.  If it does not exist, returns NULL.
         */
        virtual const char* getProperty( const std::string& name ) const ;
      
        /**
         * Looks up the value for the given property.
         * @param name the name of the property to be looked up.
         * @param defaultValue The value to be returned if the given
         * property does not exist.
         * @return The value of the property specified by <code>name</code>, if it
         * exists, otherwise the <code>defaultValue</code>.
         */
        virtual std::string getProperty( 
            const std::string& name, 
            const std::string& defaultValue ) const ;
      
        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         * @param name The name of the value to be written.
         * @param value The value to be written.
         */
        virtual void setProperty( 
            const std::string& name, 
            const std::string& value ) ;
      
        /**
         * Check to see if the Property exists in the set
         * @param name the name of the property to check
         * @return true if property exists, false otherwise.
         */
        virtual bool hasProperty( const std::string& name ) const ;

        /**
         * Removes the property with the given name.
         * @param name the name of the property to be removed.s
         */
        virtual void remove( const std::string& name ) ;
        
        /**
         * Method that serializes the contents of the property map to
         * an arryay.
         * @return list of pairs where the first is the name and the second
         * is the value.
         */
        virtual std::vector< std::pair< std::string, std::string > > toArray() const ;
      
        /**
         * Copies the contents of the given properties object to this one.
         * @param source The source properties object.
         */
        virtual void copy( const Properties* source ) ;
      
        /**
         * Clones this object.
         * @returns a replica of this object.
         */
        virtual Properties* clone() const ;
      
        /**
         * Clears all properties from the map.
         */
        virtual void clear() ;
        
     /**
     * Reads a property list (key and element pairs) from the input
     * stream.  The stream is assumed to be using the ISO 8859-1
     * character encoding; that is each byte is one Latin1 character.
     * Characters not in Latin1, and certain special characters, can
     * be represented in keys and elements using escape sequences
     * similar to those used for character and string literals (see <a
     * href="http://java.sun.com/docs/books/jls/second_edition/html/lexical.doc.html#100850">&sect;3.3</a>
     * and <a
     * href="http://java.sun.com/docs/books/jls/second_edition/html/lexical.doc.html#101089">&sect;3.10.6</a>
     * of the <i>Java Language Specification</i>).
     *
     * The differences from the character escape sequences used for
     * characters and strings are:
     *
     * <ul>
     * <li> Octal escapes are not recognized.
     *
     * <li> The character sequence <code>\b</code> does <i>not</i>
     * represent a backspace character.
     *
     * <li> The method does not treat a backslash character,
     * <code>\</code>, before a non-valid escape character as an
     * error; the backslash is silently dropped.  For example, in a
     * Java string the sequence <code>"\z"</code> would cause a
     * compile time error.  In contrast, this method silently drops
     * the backslash.  Therefore, this method treats the two character
     * sequence <code>"\b"</code> as equivalent to the single
     * character <code>'b'</code>.
     *
     * <li> Escapes are not necessary for single and double quotes;
     * however, by the rule above, single and double quote characters
     * preceded by a backslash still yield single and double quote
     * characters, respectively.
     *
     * </ul>
     *
     * An <code>IllegalArgumentException</code> is thrown if a
     * malformed Unicode escape appears in the input.
     *
     * <p>
     * This method processes input in terms of lines.  A natural line
     * of input is terminated either by a set of line terminator
     * characters (<code>\n</code> or <code>\r</code> or
     * <code>\r\n</code>) or by the end of the file.  A natural line
     * may be either a blank line, a comment line, or hold some part
     * of a key-element pair.  The logical line holding all the data
     * for a key-element pair may be spread out across several adjacent
     * natural lines by escaping the line terminator sequence with a
     * backslash character, <code>\</code>.  Note that a comment line
     * cannot be extended in this manner; every natural line that is a
     * comment must have its own comment indicator, as described
     * below.  If a logical line is continued over several natural
     * lines, the continuation lines receive further processing, also
     * described below.  Lines are read from the input stream until
     * end of file is reached.
     *
     * <p>
     * A natural line that contains only white space characters is
     * considered blank and is ignored.  A comment line has an ASCII
     * <code>'#'</code> or <code>'!'</code> as its first non-white
     * space character; comment lines are also ignored and do not
     * encode key-element information.  In addition to line
     * terminators, this method considers the characters space
     * (<code>' '</code>, <code>'&#92;u0020'</code>), tab
     * (<code>'\t'</code>, <code>'&#92;u0009'</code>), and form feed
     * (<code>'\f'</code>, <code>'&#92;u000C'</code>) to be white
     * space.
     *
     * <p>
     * If a logical line is spread across several natural lines, the
     * backslash escaping the line terminator sequence, the line
     * terminator sequence, and any white space at the start the
     * following line have no affect on the key or element values.
     * The remainder of the discussion of key and element parsing will
     * assume all the characters constituting the key and element
     * appear on a single natural line after line continuation
     * characters have been removed.  Note that it is <i>not</i>
     * sufficient to only examine the character preceding a line
     * terminator sequence to see if the line terminator is
     * escaped; there must be an odd number of contiguous backslashes
     * for the line terminator to be escaped.  Since the input is
     * processed from left to right, a non-zero even number of
     * 2<i>n</i> contiguous backslashes before a line terminator (or
     * elsewhere) encodes <i>n</i> backslashes after escape
     * processing.
     *
     * <p>
     * The key contains all of the characters in the line starting
     * with the first non-white space character and up to, but not
     * including, the first unescaped <code>'='</code>,
     * <code>':'</code>, or white space character other than a line
     * terminator. All of these key termination characters may be
     * included in the key by escaping them with a preceding backslash
     * character; for example,<p>
     *
     * <code>\:\=</code><p>
     *
     * would be the two-character key <code>":="</code>.  Line
     * terminator characters can be included using <code>\r</code> and
     * <code>\n</code> escape sequences.  Any white space after the
     * key is skipped; if the first non-white space character after
     * the key is <code>'='</code> or <code>':'</code>, then it is
     * ignored and any white space characters after it are also
     * skipped.  All remaining characters on the line become part of
     * the associated element string; if there are no remaining
     * characters, the element is the empty string
     * <code>&quot;&quot;</code>.  Once the raw character sequences
     * constituting the key and element are identified, escape
     * processing is performed as described above.
     *
     * <p>
     * As an example, each of the following three lines specifies the key
     * <code>"Truth"</code> and the associated element value
     * <code>"Beauty"</code>:
     * <p>
     * <pre>
     * Truth = Beauty
     *	Truth:Beauty
     * Truth			:Beauty
     * </pre>
     * As another example, the following three lines specify a single
     * property:
     * <p>
     * <pre>
     * fruits                           apple, banana, pear, \
     *                                  cantaloupe, watermelon, \
     *                                  kiwi, mango
     * </pre>
     * The key is <code>"fruits"</code> and the associated element is:
     * <p>
     * <pre>"apple, banana, pear, cantaloupe, watermelon, kiwi, mango"</pre>
     * Note that a space appears before each <code>\</code> so that a space
     * will appear after each comma in the final result; the <code>\</code>,
     * line terminator, and leading white space on the continuation line are
     * merely discarded and are <i>not</i> replaced by one or more other
     * characters.
     * <p>
     * As a third example, the line:
     * <p>
     * <pre>cheeses
     * </pre>
     * specifies that the key is <code>"cheeses"</code> and the associated
     * element is the empty string <code>""</code>.<p>
     *
     * @param      inStream   the input stream.
     * @exception  IOException  if an error occurred when reading from the
     *               input stream.
     * @throws	   IllegalArgumentException if the input stream contains a
     * 		   malformed Unicode escape sequence.
     */

        virtual void load(org::esb::io::InputStream * is) ;
        virtual void save(org::esb::io::OutputStream * os) ;

   };
   
}}}

#endif 
