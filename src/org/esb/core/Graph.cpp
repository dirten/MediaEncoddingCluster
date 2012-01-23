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
#include "TaskException.h"
#include "GraphException.h"
#include "PluginContext.h"

#include <list>
namespace org {
  namespace esb {
    namespace core {

     // Graph::Graph() {
     // }

      Graph::Graph(std::string graph) {
        throw GraphException("Constructor not supported", "-1");
      }

      Graph::Graph(std::list<Ptr<Graph::Element> > data, std::string uuid) {
        elements = data;
        _uuid = uuid;
        _state = Graph::NONE;
        haserror = false;
        _isCanceled = false;
        //status=JSONNode(JSON_NODE);
        //status.set_name("status");
        JSONNode test(JSON_NODE);
        status.push_back(test);
        processedStepCount = 0;
        test.push_back(JSONNode("test", "bla"));
      }

      Graph::~Graph() {

      }

      int Graph::getStepCount() {
        return elements.size();
      }

      int Graph::getProcessedStepCount() {
        return processedStepCount;
      }

      void Graph::cancel() {
        /*if (_current_task)
          _current_task->cancel();
        _isCanceled = true;*/
      }

      void Graph::run() {
        _state = EXECUTE;

        foreach(Ptr<Graph::Element> el, elements) {
          if (_isCanceled)return;
          if (el->getParents().size() == 0) {
            execute(el);
          }
        }
        if (haserror) {
          _state = DONE_WITH_ERROR;
        } else if (_isCanceled) {
          _state = CANCELED;
        } else {
          _state = DONE;
        }
      }

      void Graph::execute(Ptr<Element> e) {
        LOGDEBUG("Context:" << e->task->getContext()->toString());

        //KeyValue s;//=new KeyValue();
        Ptr<Status> s(new Status());
        //s->progress="0";

        status_list.push_back(s);
        //status_list.push_back(s);
        s->uid = e->id;
        status_list.back()->message = "";
        try {
          Ptr<org::esb::core::Task>task = e->task;
            //_current_task = task;
            task->setUUID(_uuid);

            task->addProgressObserver(boost::bind(&Graph::setProgress, this, _1));
            task->addStatusObserver(boost::bind(&Graph::setStatus, this, _1));

            task->setSink(org::esb::util::PUUID());
            task->prepare();
            //exec.push_back(JSONNode("status", "prepared"));


            //status.push_back(exec);

            //exec.push_back(JSONNode("status", "running"));
            //go(Graph::setProgress,this,task,s);
            //boost::thread(boost::bind(&Graph::setProgress,this,task,s));
            task->execute();
            //s->progress="100";
            //exec.push_back(JSONNode("status", "cleanup"));

            task->cleanup();
            status_list.back()->message = "Completed";
            //setStatus(e->task.get());
            //exec.push_back(JSONNode("status", "finished"));

            //status_list.remove(s);
            //s["status"] = "finished";
            //status_list.push_back(s);
            //setProgress(task,s);
          processedStepCount++;
          setStatus(e->task.get());

          if (e->task->getStatus() != org::esb::core::Task::ERROR) {
            foreach(Ptr<Graph::Element> el, e->getChilds()) {
              el->task->setSource(e->task->getSink());
              el->task->getContext()->merge(e->task->getContext());
              execute(el);
            }
          }
        } catch (org::esb::core::TaskException & ex) {
          status_list.back()->message = "";
          status_list.back()->exception = ex.displayText();
          status_list.back()->status = org::esb::core::Task::ERROR;
          if (statusObserver)
            statusObserver(this);
          haserror = true;
          //throw GraphException(ex.displayText(), e->id);
        }
      }

      Graph::STATUS Graph::getState() {
        return _state;
      }

      void Graph::addStatusObserver(boost::function<void (Graph*) > func) {
        statusObserver = func;
      }

      void Graph::setProgress(Task* t) {
        status_list.back()->progress = org::esb::util::StringUtil::toString(t->getProgress());
        if (statusObserver)
          statusObserver(this);
      }

      void Graph::setStatus(Task * t) {
        status_list.back()->status = t->getStatus();
        status_list.back()->message = t->getStatusMessage();
        if (statusObserver)
          statusObserver(this);
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
          n.push_back(JSONNode("message", s->message));
          n.push_back(JSONNode("exception", s->exception));
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
