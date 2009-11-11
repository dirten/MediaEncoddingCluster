#ifndef ORG_ESB_HIVE_JOB_PROCESS_UNIT_H
#define ORG_ESB_HIVE_JOB_PROCESS_UNIT_H

#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include <list>
#include <set>
//#include <multiset>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "org/esb/util/Log.h"

using namespace std;
using namespace org::esb::av;
namespace org {
    namespace esb {
        namespace hive {
            namespace job {
                class ProcessUnit {
                public:
                    logger("hive.processunit");
                    ProcessUnit();
                    ~ProcessUnit();
                    void process();
                    std::string toString();
                    //	private:
                    //	        friend class boost::serialization::access;

                    std::list<boost::shared_ptr<Packet> > _input_packets;
                    std::list<boost::shared_ptr<Packet> > _output_packets;
                    Decoder * _decoder;
                    Encoder * _encoder;
                    Codec * codec;
                    int _source_stream;
                    int _target_stream;
                    int _frame_group;
                    int _frame_count;
                    int _process_unit;
                    int id;
                   
                    bool _last_process_unit;
                    template<class Archive>
                    void serialize(Archive & ar, const unsigned int version) {
                        ar & _input_packets;
                        ar & _output_packets;
                        ar & _source_stream;
                        ar & _target_stream;
                        ar & _frame_group;
                        ar & _frame_count;
                        ar & _process_unit;
                        ar & _decoder;
                        ar & _encoder;
                        ar & codec;
                        ar & _last_process_unit;
                    }
                private:
                    
                };

                class PtsComparator {
                public:

                    bool operator()(const boost::shared_ptr<Frame> & lp, const boost::shared_ptr<Frame> & rp)const {
                        return lp->getDts() < rp->getDts();
                    }
                };

                class PtsPacketComparator {
                public:

                    bool operator()(const boost::shared_ptr<Packet> & lp, const boost::shared_ptr<Packet> & rp)const {
                        return lp->getAVPacket()->dts < rp->getAVPacket()->dts;
                    }
                };


            }
        }
    }
}

#endif
