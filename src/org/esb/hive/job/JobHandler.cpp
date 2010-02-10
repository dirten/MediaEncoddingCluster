/*----------------------------------------------------------------------
 *  File    : JobHandler.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#include "JobHandler.h"
#include "Job.h"
#include "JobWatcher.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/lang/Thread.h"
#include <iostream>
using namespace org::esb::hive::job;
using namespace org::esb::io;

JobHandler * JobHandler::_handler=0;



JobHandler::JobHandler(){
//	JobWatcher *_watcher=new JobWatcher(*this);
//	Thread *runner=new Thread(_watcher);
//	runner->start();
	
}
list<Job*> JobHandler::getJobList(){
	return _jobList;
}

Job * JobHandler::getJob(){
//	boost::mutex::scoped_lock scoped_lock(m_mutex);
        list<Job*>::iterator i;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
	    	if(tmp->getCompleteTime()==0){
				return tmp;
	    	}
        }
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
	    	if(tmp->getCompleteTime()==0&&tmp->_stream_type==CODEC_TYPE_AUDIO){
				return tmp;
	    	}
        }


    return NULL;
}

Job * JobHandler::getJob(int id){
        list<Job*>::iterator i;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
	    if(tmp->getId()==id){
		return tmp;
	    }
        }
        return NULL;
}

bool JobHandler::addJob(Job & job){
        list<Job*>::iterator i;
        bool toAdd=true, result=false;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
			if(tmp->getId()==job.getId()){
				toAdd=false;
			}
        }
        if(toAdd){
        	job.activate();
        	_jobList.push_back(&job);
        	
//        	cout << "Job with ID added:"<<job.getId()<<endl;
        	result=true;
        }
        return result;
}

JobHandler * JobHandler::getInstance(){
    if(_handler==NULL){
		JobHandler::_handler=new JobHandler();
    }
    return _handler;
}
