/* 
 * File:   Graph.h
 * Author: jhoelscher
 *
 * Created on 11. Dezember 2011, 15:14
 */
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Task.h"
#include "exports.h"
#include "org/esb/libjson/libjson.h"
//#include "GraphParser.h"

#include <list>
#include <map>
#ifndef GRAPH_H
#define	GRAPH_H
namespace org {
  namespace esb {
    namespace core {
      
      class CORE_EXPORT Graph  {
      public:
        class Element{
        public:
          ~Element(){
            dependsOn.clear();
          }
          void setTask(Ptr<org::esb::core::Task>t){task=t;}
          void addChild(Ptr<Element>  el){linksTo.push_back(el);}
          void addParent(Ptr<Element>  el){dependsOn.push_back(el->id);}
          std::list<Ptr<Element> > getChilds(){return linksTo;}
          std::list<std::string> getParents(){return dependsOn;}
          Ptr<org::esb::core::Task> task;
        private:
          std::list<Ptr<Element> > linksTo;
          std::list<std::string> dependsOn;
        public:
          bool finished;
          bool visited;
          std::string parameter;
          std::string name;
          std::string id;
        };
        enum STATUS {
          NONE,
          PREPARE,
          EXECUTE,
          CLEANUP,
          DONE,
          INTERRUPT,
          INTERRUPTED,
          ERROR,
          DONE_WITH_ERROR
        };

        Graph(std::string);
        Graph(std::list<Ptr<Graph::Element> >, std::string uuid="");
        static void createJob(std::list<Ptr<Graph::Element> >,boost::shared_ptr<db::HiveDb>);
        void addElement(Element);
        void addElement(Ptr<Task>);
        void linkElement(Ptr<Task>,Ptr<Task>);
        void run();
        void addStatusObserver(boost::function<void (Graph*)> func);
        std::string getStatus();
        STATUS getState();
          int getStepCount();
          int getProcessedStepCount();
        virtual ~Graph();
      private:
        boost::function<void (Graph*)> statusObserver;
        static void processElement(Ptr<Element> ,db::Job & );
        std::list<Ptr<Graph::Element> > elements;
        void execute(Ptr<Element>);
        JSONNode status;
        std::string _uuid;
        STATUS _state;
        int processedStepCount;
        bool haserror;
        class Status{
        public:
          Status(){
            uid="";
            progress="0";
            status=Task::NONE;
            message="";
          }
          std::string uid;
          Task::STATUS status;
          std::string progress;
          std::string message;
        };
        
        typedef std::map<std::string, std::string> KeyValue;
        typedef std::list<Ptr<Status> > KeyValueList;
        KeyValueList status_list;
        void setStatus(Task*);
        void setProgress(Task*);
        //std::set<Ptr<Task> > _taskList;
        //std::list<Ptr<Task> > _taskLinkMap;
        //std::string _link_matrix[100][100];
      };
    }
  }
}

#endif	/* GRAPH_H */

