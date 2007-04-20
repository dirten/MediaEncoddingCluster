#ifndef ORG_ESB_UTIL_PROPERTIES_H_
#define ORG_ESB_UTIL_PROPERTIES_H_

#include <map>
#include <string>
#include <vector>

namespace org{
namespace esb{
namespace util{
   
    /**
     * Interface for a Java-like properties object.  This is essentially 
     * a map of key-value string pairs.
     */
    class Properties{
    public:
   
        virtual ~Properties(){}
        
        /**
         * Returns true if the properties object is empty
         * @return true if empty
         */
        virtual bool isEmpty() const = 0;
        
        /**
         * Looks up the value for the given property.
         * @param name The name of the property to be looked up.
         * @return the value of the property with the given name, if it
         * exists.  If it does not exist, returns NULL.
         */
        virtual const char* getProperty( const std::string& name ) const = 0;
      
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
            const std::string& defaultValue ) const = 0;
      
        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         * @param name The name of the value to be written.
         * @param value The value to be written.
         */
        virtual void setProperty( 
            const std::string& name, 
            const std::string& value ) = 0;
      
        /**
         * Check to see if the Property exists in the set
         * @param name the name of the property to check
         * @return true if property exists, false otherwise.
         */
        virtual bool hasProperty( const std::string& name ) const = 0;

        /**
         * Removes the property with the given name.
         * @param name the name of the property to be removed.s
         */
        virtual void remove( const std::string& name ) = 0;
        
        /**
         * Method that serializes the contents of the property map to
         * an arryay.
         * @return list of pairs where the first is the name and the second
         * is the value.
         */
        virtual std::vector< std::pair< std::string, std::string > > toArray() const = 0;
      
        /**
         * Copies the contents of the given properties object to this one.
         * @param source The source properties object.
         */
        virtual void copy( const Properties* source ) = 0;
      
        /**
         * Clones this object.
         * @returns a replica of this object.
         */
        virtual Properties* clone() const = 0;
      
        /**
         * Clears all properties from the map.
         */
        virtual void clear() = 0;

   };
   
}}}

#endif 
