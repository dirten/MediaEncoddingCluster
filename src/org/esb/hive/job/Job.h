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
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
using namespace std;
using namespace org::esb::util;
using namespace org::esb::av;
using namespace org::esb::sql;
namespace org{
namespace esb{
namespace hive{
namespace job{
class Job{
    public:
		Job();
		~Job();
		int getId();
		void setId(int id);
		int getStartTime();
		int getCompleteTime();
		void setStartTime(int start);
		void setCompleteTime(int complete);
		void setSourceStream(int s);
		void setTargetStream(int s);
		int getSourceStream();
		int getTargetStream();
/*		File & getSourceFile(void);
		File & getTargetFile(void);
		void setSourceFile(File& source);
		void setTargetFile(File& target);
		void addJobDetails(JobDetail & detail);
		*/
//		bool getNextProcessUnit(ProcessUnit & unit);
		ProcessUnit getNextProcessUnit();
		queue<ProcessUnit*> _unit_queue;
//		static int process(void *NotUsed, int argc, char **argv, char **azColName);
	private:
		friend class JobProcess;
//		File * _source;
//		File * _target;
		int _startTime;
		int _completeTime;
		int _id;
		int _source_stream;
		int _target_stream;
		
//		vector<JobDetail*>_detailList;
		int _frame_group;
		mutable boost::mutex m_mutex;
		Connection * _con;
		Statement * _stmt;
		Decoder * _decoder;
		Encoder * _encoder;
//		static ProcessUnit * _unit;
};	
}}}}
#endif

