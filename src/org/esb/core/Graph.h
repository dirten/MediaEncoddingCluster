/* 
 * File:   Graph.h
 * Author: jhoelscher
 *
 * Created on 11. Dezember 2011, 15:14
 */
#include "org/esb/lang/Ptr.h"
#include "Task.h"
#include "exports.h"
#ifndef GRAPH_H
#define	GRAPH_H
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT Graph {
      public:
        Graph();
        void addElement(Ptr<Task>);
        void linkElement(Ptr<Task>,Ptr<Task>);
        virtual ~Graph();
      private:

      };
    }
  }
}

#endif	/* GRAPH_H */

