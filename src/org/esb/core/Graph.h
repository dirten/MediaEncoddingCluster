/* 
 * File:   Graph.h
 * Author: jhoelscher
 *
 * Created on 11. Dezember 2011, 15:14
 */
#include "org/esb/lang/Ptr.h"
#include "Task.h"
#include "exports.h"

#include <set>
#include <map>
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
        std::set<Ptr<Task> > _taskList;
        //std::list<Ptr<Task> > _taskLinkMap;
        std::string _link_matrix[100][100];
      };
    }
  }
}

#endif	/* GRAPH_H */

