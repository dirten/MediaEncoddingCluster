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
namespace org {
  namespace esb {
    namespace core {

      class GraphParser {
      public:
        typedef std::map<std::string, Ptr<Graph::Element> > ElementMap;
        GraphParser(std::string) throw(GraphException);
        virtual ~GraphParser();
        ElementMap & getElementMap();
      private:
        void parse(JSONNode&);
        void parseTask(JSONNode&);
        void parseLink(JSONNode&);
        void verifyLinks();
        void verifyCycle();
        void verifyCycle(Ptr<Graph::Element> element);
        
        ElementMap elements;

      };
    }
  }
}

#endif	/* GRAPHPARSER_H */

