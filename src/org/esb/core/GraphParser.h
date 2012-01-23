/* 
 * File:   GraphParser.h
 * Author: jhoelscher
 *
 * Created on 12. Dezember 2011, 10:34
 */

#ifndef GRAPHPARSER_H
#define	GRAPHPARSER_H
#include "org/esb/libjson/libjson.h"
#include "Graph.h"
#include "GraphException.h"
#include <map>
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT GraphParser {
      public:
        typedef std::map<std::string, Ptr<Graph::Element> > ElementMap;
        GraphParser(std::string, std::string infile="") throw (GraphException);
        virtual ~GraphParser();
        ElementMap & getElementMap();
        std::string getName();
        std::string getInfile();
        void setInfile(std::string);
        std::string getGraphString();
      private:
        void parse(JSONNode&);
        void parseTask(JSONNode&);
        void parseLink(JSONNode&);
        void verifyLinks();
        void verifyCycle();
        void verifyCycle(Ptr<Graph::Element> element);

        ElementMap elements;
        std::string name;
        std::string infile;
        JSONNode _baseNode;

      };
    }
  }
}

#endif	/* GRAPHPARSER_H */

