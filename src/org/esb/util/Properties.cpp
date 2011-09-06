#ifndef ORG_ESB_UTIL_SIMPLEPROPERTIES_H_
#define ORG_ESB_UTIL_SIMPLEPROPERTIES_H_

#include <map>
#include <string>
#include <iostream>
#include <org/esb/util/Properties.h>
#include <org/esb/util/StringTokenizer.h>
//class LineReader(org::esb::io::InputStream);
using namespace std;
namespace org {
  namespace esb {
    namespace util {
      class LineReader {
      private:
        org::esb::io::InputStream * _is;
        string _line;
      public:
        LineReader(org::esb::io::InputStream * is) {
          _is = is;
        }
        string readLine() {
          _line.clear();
          char c;
          while ((c = _is->read())) {
            _line += c;
            if (c == '\n') {
              _line[_line.size() - 1] = '\0';
              break;
            }
          }
          return _line;
        }
      };

      /**
       * Basic implementation of the Properties interface.
       */

      Properties::~Properties() {
      }

      /**
       * Returns true if the properties object is empty
       * @return true if empty
       */
      bool Properties::isEmpty() const {
        return properties.empty();
      }

      /**
       * Looks up the value for the given property.
       * @param name The name of the property to be looked up.
       * @return the value of the property with the given name, if it
       * exists.  If it does not exist, returns NULL.
       */
      const char* Properties::getProperty(const std::string& name) const {

        std::map<std::string, std::string>::const_iterator iter =
            properties.find(name);
        if (iter == properties.end()) {
          return NULL;
        }

        return iter->second.c_str();
      }

      /**
       * Looks up the value for the given property.
       * @param name the name of the property to be looked up.
       * @param defaultValue The value to be returned if the given
       * property does not exist.
       * @return The value of the property specified by <code>name</code>, if it
       * exists, otherwise the <code>defaultValue</code>.
       */
      std::string Properties::getProperty(const std::string& name,
          const std::string& defaultValue) const {

        std::map<std::string, std::string>::const_iterator iter =
            properties.find(name);
        if (iter == properties.end()) {
          return defaultValue;
        }

        return iter->second;
      }

      /**
       * Sets the value for a given property.  If the property already
       * exists, overwrites the value.
       * @param name The name of the value to be written.
       * @param value The value to be written.
       */
      void Properties::setProperty(const std::string name,
          const std::string value) {
        properties[name] = value;
      }

      /**
       * Check to see if the Property exists in the set
       * @param name - property name to check for in this properties set.
       * @return true if property exists, false otherwise.
       */
      bool Properties::hasProperty(const std::string name) const {
        if (properties.find(name) != properties.end()) {
          return true;
        }

        return false;
      }

      /**
       * Removes the property with the given name.
       * @param name the name of the property to remove.
       */
      void Properties::remove(const std::string& name) {
        properties.erase(name);
      }

      /**
       * Method that serializes the contents of the property map to
       * an arryay.
       * @return list of pairs where the first is the name and the second
       * is the value.
       */
      std::vector<std::pair<std::string, std::string> > Properties::toArray() const {

        // Create a vector big enough to hold all the elements in the map.
        std::vector<std::pair<std::string, std::string> >
            vec(properties.size());

        // Get an iterator at the beginning of the map.
        std::map<std::string, std::string>::const_iterator iter =
            properties.begin();

        // Copy all of the elements from the map to the vector.
        for (int ix = 0; iter != properties.end(); ++iter, ++ix) {
          vec[ix] = *iter;
        }

        return vec;
      }

      /**
       * Copies the contents of the given properties object to this one.
       * @param source The source properties object.
       */
      void Properties::copy(const Properties* source) {

        clear();

        std::vector<std::pair<std::string, std::string> > vec =
            source->toArray();
        for (unsigned int ix = 0; ix < vec.size(); ++ix) {
          properties[vec[ix].first] = vec[ix].second;
        }
      }

      /**
       * Clones this object.
       * @returns a replica of this object.
       */
      Properties* Properties::clone() const {

        Properties* props = new Properties();

        props->properties = properties;

        return props;
      }

      /**
       * Clears all properties from the map.
       */
      void Properties::clear() {
        properties.clear();
      }

      void Properties::load(org::esb::io::InputStream * is) {
        LineReader lineReader(is);
        while (true) {
          string line = lineReader.readLine();
          if(line.length()<=1)break;
//          cout << "line" << line <<":"<<line.length()<< endl;

          StringTokenizer st(line, "=");
          if (st.countTokens() == 2) {
            string key = st.nextToken();
            string val = st.nextToken();
            setProperty(key, val);
//            cout << "Key:" << key << "\tVal:" << val << endl;
          }
        }
      }
      void Properties::save(org::esb::io::OutputStream * os) {
        std::map<std::string, std::string>::const_iterator iter =
            properties.begin();
        for (; iter != properties.end(); iter++) {
          os->write((char *) iter->first.c_str(), iter->first.length());
          os->write((char*) "=", 1);
          os->write((char *) iter->second.c_str(), iter->second.length());
          os->write((char*) "\n", 1);
        }
        os->flush();
      }
    }
  }
}

#endif 
