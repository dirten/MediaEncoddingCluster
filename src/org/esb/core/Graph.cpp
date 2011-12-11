/* 
 * File:   Graph.cpp
 * Author: jhoelscher
 * 
 * Created on 11. Dezember 2011, 15:14
 */

#include <list>

#include "Graph.h"
namespace org {
  namespace esb {
    namespace core {

      Graph::Graph() {
      }

      Graph::~Graph() {
      }
      
      void Graph::addElement(Ptr<Task> element){
        _taskList.insert(element);
        //_link_matrix[_taskList.size()][0]="";
      }

      void Graph::linkElement(Ptr<Task> from,Ptr<Task> to){
        if(_taskList.count(from)>0){
          if(_taskList.count(to)>0){
           
          }
        }
      }
    }
  }
}
