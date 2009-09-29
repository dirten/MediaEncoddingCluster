#ifndef ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#define ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/util/Log.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"

#include <map>
#include <deque>
#include <boost/shared_ptr.hpp>
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        class ProcessUnitWatcher : public org::esb::signal::MessageListener {
          logger("hive.ProcessUnitWatcher")
        public:
          ProcessUnitWatcher();
          //			void run();
          void start();
          void start2();
          void start3();
          void stop();
          void onMessage(org::esb::signal::Message & msg);
        private:
          bool _isStopSignal;
          std::map<int,boost::shared_ptr<ProcessUnit> > unit_map;
          std::map<int, int> idx;
          std::map<int, int> inout;
          std::map<int, int> stream_type;
          void readJobs();
          void processAudioPacket(boost::shared_ptr<Packet>);
          void processVideoPacket(boost::shared_ptr<Packet>);
          std::map<int,std::list<boost::shared_ptr<Packet> > > stream_packets;
          std::deque<boost::shared_ptr<Packet> > packet_queue;
          std::map<int, int> stream_packet_counter;
          int min_frame_group_count;
          int b_frame_offset;
          bool q_filled;
          int job_id;
          org::esb::sql::Connection * _con_tmp;
          org::esb::sql::PreparedStatement * _stmt;
          void flushStreamPackets();
          void buildProcessUnit(int sIdx);
        };

      }
    }
  }
}
#endif



