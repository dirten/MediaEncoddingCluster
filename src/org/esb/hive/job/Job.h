#ifndef ORG_ESB_HIVE_JOB_JOB_H
#define ORG_ESB_HIVE_JOB_JOB_H
#include "org/esb/util/Properties.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include <boost/thread/mutex.hpp>
#include "JobDetail.h"
#include "ProcessUnit.h"
#include <queue>
#include <vector>

using namespace std;
using namespace org::esb::util;
using namespace org::esb::av;
namespace org{
namespace esb{
namespace hive{
namespace job{
class Job{
    public:
		Job();
		~Job();
		File & getSourceFile(void);
		File & getTargetFile(void);
		void setSourceFile(File& source);
		void setTargetFile(File& target);
		int getStartTime();
		int getCompleteTime();
		void setStartTime(int start);
		void setCompleteTime(int complete);
		int getId();
		void setId(int id);
		void addJobDetails(JobDetail & detail);
		bool getNextProcessUnit(ProcessUnit & unit);
		queue<ProcessUnit*> _unit_queue;
//		static int process(void *NotUsed, int argc, char **argv, char **azColName);
	private:
		friend class JobProcess;
		File * _source;
		File * _target;
		int _startTime;
		int _completeTime;
		int _id;
		vector<JobDetail*>_detailList;
		int _frame_group;
		mutable boost::mutex m_mutex;
//		static ProcessUnit * _unit;
};	
}}}}
#endif

