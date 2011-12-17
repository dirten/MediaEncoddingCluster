/* 
 * File:   Graph.cpp
 * Author: jhoelscher
 * 
 * Created on 11. Dezember 2011, 15:14
 */
#include "Graph.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/UUID.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/lang/Thread.h"
#include <list>
namespace org {
  namespace esb {
    namespace core {

      Graph::Graph(std::string graph) {
      }

      Graph::Graph(std::list<Ptr<Graph::Element> > data, std::string uuid) {
        elements = data;
        _uuid = uuid;
        _state=Graph::NONE;
        //status=JSONNode(JSON_NODE);
        //status.set_name("status");
        JSONNode test(JSON_NODE);
        status.push_back(test);

        test.push_back(JSONNode("test", "bla"));
      }

      Graph::~Graph() {

      }

      void Graph::run() {
        _state=EXECUTE;
        foreach(Ptr<Graph::Element> el, elements) {
          if (el->getParents().size() == 0) {
            try{
            execute(el);
            }catch(std::exception & ex){
              _state=ERROR;
              throw ex;
            }
            //processElement(el, job);
          }
        }
        _state=DONE;
      }

      void Graph::execute(Ptr<Element> e) {
        //KeyValue s;//=new KeyValue();
        Ptr<Status> s(new Status());
        
        status_list.push_back(s);
        //status_list.push_back(s);
        s->uid=e->id;
        s->status="prepare";

        Ptr<org::esb::core::Task>task = e->task;
        task->setSink(org::esb::util::PUUID());
        task->prepare();
        //exec.push_back(JSONNode("status", "prepared"));
        
        s->status="prepare";

        task->setUUID(_uuid);
        //status.push_back(exec);

        //exec.push_back(JSONNode("status", "running"));
        s->status="running";
        go(Graph::setProgress,this,task,s);
        //boost::thread(boost::bind(&Graph::setProgress,this,task,s));
        task->execute();
        s->progress="100";
        //exec.push_back(JSONNode("status", "cleanup"));
        s->status="cleanup";

        task->cleanup();
        //exec.push_back(JSONNode("status", "finished"));
        s->status="done";

        //status_list.remove(s);
        //s["status"] = "finished";
        //status_list.push_back(s);

        if (task->getStatus() != org::esb::core::Task::ERROR) {

          foreach(Ptr<Graph::Element> el, e->getChilds()) {
            el->task->setSource(task->getSink());
            execute(el);
          }
        }
      }
      Graph::STATUS Graph::getState(){
        return _state;
      }
      void Graph::setProgress(Ptr<Task> t, Ptr<Status> s){
        using namespace org::esb::core;
        while(t->getStatus()!=Task::DONE&&t->getStatus()!=Task::ERROR){
          s->progress=org::esb::util::StringUtil::toString(t->getProgress());
        //setStatus(s,"progress",org::esb::util::StringUtil::toString(t->getProgress()));
        org::esb::lang::Thread::sleep2(1000);
        }
      }
      
      void Graph::setStatus(Ptr<Status> s, std::string key, std::string val){
        //status_list.remove(s);
        //s[key] = val;
        //status_list.push_back(s);       
      }
      
      std::string Graph::getStatus() {
        JSONNode node(JSON_NODE);

        foreach(Ptr<Status> s, status_list) {
          JSONNode n(JSON_NODE);
          n.set_name(s->uid);
          n.push_back(JSONNode("status", s->status));
          n.push_back(JSONNode("progress", s->progress));
          node.push_back(n);
        }
        //status.preparse();
        return node.write_formatted();
      }

      void Graph::createJob(std::list<Ptr<Graph::Element> > els, boost::shared_ptr<db::HiveDb> db) {
        db->begin();
        db::Job job(*db.get());
        job.uuid = (std::string)org::esb::util::PUUID();
        job.update();

        foreach(Ptr<Graph::Element> el, els) {
          //LOGDEBUG("childsize:"<<el.linksTo.size());
          if (el->getParents().size() == 0) {
            processElement(el, job);
          }
        }
        db->commit();
      }

      void Graph::processElement(Ptr<Element> el, db::Job & job) {
        LOGDEBUG("void Graph::processElement(Element&el,db::Job & job)");
        LOGDEBUG("Element id:" << el->id);
        LOGDEBUG("childsize:" << el->getChilds().size());
        db::Task task(job.getDatabase());
        task.uuid = (std::string)org::esb::util::PUUID();
        task.name = el->name;
        task.parameter = el->parameter;
        task.update();
        job.tasks().link(task);

        foreach(Ptr<Graph::Element> e, el->getChilds()) {
          processElement(e, job);
        }
      }

      void Graph::addElement(Ptr<Task> element) {

      }

      void Graph::linkElement(Ptr<Task> from, Ptr<Task> to) {

      }
    }
  }
}
