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
#ifndef ORG_ESB_HIVE_JOB_JOBHANDLER_H
#define ORG_ESB_HIVE_JOB_JOBHANDLER_H

#include "Job.h"
#include "org/esb/util/Log.h"
#include <list>
#include <boost/thread/mutex.hpp>
namespace org{
namespace esb{
namespace hive{
namespace job{
class JobHandler{
    private:
	logger("hive.jobhandler");
	static JobHandler * _handler;
	JobHandler();
	list<Job*>_jobList;
	Job * _testJob;
	mutable boost::mutex m_mutex;

    public:
	static JobHandler * getInstance();
	Job * getJob();
	list<Job*> getJobList();
	Job * getJob(int id);
	bool addJob(Job & job);
	Properties _props;
};
}}}}
#endif
