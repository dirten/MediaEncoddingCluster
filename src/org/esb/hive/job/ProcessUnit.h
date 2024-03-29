/*----------------------------------------------------------------------
 *  File    : ProcessUnit.h
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
#ifndef ORG_ESB_HIVE_JOB_PROCESS_UNIT_H
#define ORG_ESB_HIVE_JOB_PROCESS_UNIT_H

#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"
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
#include "boost/date_time/posix_time/posix_time.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Deprecated.h"
//#include "cereal/cereal.hpp"
#include "../../core/exports.h"
using namespace std;
using namespace org::esb::av;
using namespace boost::posix_time;
namespace org {
    namespace esb {
        namespace hive {
            namespace job {
                class CORE_EXPORT ProcessUnit {
                public:
                    classlogger("org.esb.hive.job.ProcessUnit")
                    ProcessUnit();
                    ~ProcessUnit();
                    void process();
                    boost::shared_ptr<Decoder> getDecoder();
                    boost::shared_ptr<Encoder> getEncoder();
                    std::list<boost::shared_ptr<Packet> > getInputPacketList();
                    std::list<boost::shared_ptr<Packet> > getOutputPacketList();
                    void setJobId(std::string uuid);
                    std::string getJobId();
                    int getGopSize();
                    int getExpectedFrameCount();
                    std::string toString();
                    void setProperty(std::string, std::string);
                    std::string getProperty(std::string);
                    bool hasProperty(std::string);
                    void setFps(int);
                    int getFps();
                    Ptr<org::esb::av::AVFilter>getFilter();
                    void setFilter(Ptr<org::esb::av::AVFilter> filter);
                    //	private:
                    //	        friend class boost::serialization::access;

                    std::list<boost::shared_ptr<Packet> > _input_packets;
                    std::list<boost::shared_ptr<Packet> > _output_packets;
                    boost::shared_ptr<Decoder> _decoder;
                    boost::shared_ptr<Encoder> _encoder;
                    boost::shared_ptr<Decoder> _2passdecoder;
                    boost::shared_ptr<Encoder> _2passencoder;
                    boost::shared_ptr<Decoder> _refdecoder;
                    Ptr<Decoder> _testdecoder;
//                    Codec * codec;
                    FrameConverter * _converter;
                    int _source_stream;
                    int _target_stream;
                    int _frame_group;
                    int _frame_count;
                    int _process_unit;
                    int id;
                    std::string _job_id;
                    std::string uuid;
                    double _frameRateCompensateBase;
                    int _gop_size;
                   
                    bool _last_process_unit;
                    int _expected_frame_count;
                    int _deinterlace;
                    int _keep_aspect_ratio;
                    int64_t _discard_audio_bytes;
                    int _sequence;
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
                        ar & _2passdecoder;
                        ar & _2passencoder;
                        ar & _last_process_unit;
                        ar & _frameRateCompensateBase;
                        ar & _gop_size;
                        ar & _expected_frame_count;
                        ar & _deinterlace;
                        ar & _keep_aspect_ratio;
                        ar & _discard_audio_bytes;
                        ar & _properties;
                        ar & _job_id;
                        ar & _sequence;
                        ar & _fps;
                        ar & uuid;
                    }
                private:
                  Frame * convertToRgb(Frame *);
                  DEPRECATED(void processInternal());
                  void processInternal2();
                  void processPsnr(Frame * ref, Frame * cmp);
                  void decodeLastPacket(Packet * pac);
                  std::map<std::string, std::string> _properties;
                  int calculatePsnr(Frame * ref, Frame * enc);
                  int _fps;
                  ptime _start;
                  ptime _end;
                  Ptr<org::esb::av::AVFilter> _filter;
                };
                /*
                class PtsComparator {
                public:

                    bool operator()(const boost::shared_ptr<Frame> & lp, const boost::shared_ptr<Frame> & rp)const {
                        return lp->getDts() < rp->getDts();
                    }
                };
                */
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
