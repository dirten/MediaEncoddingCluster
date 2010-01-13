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
                    boost::shared_ptr<Decoder> _decoder;
                    boost::shared_ptr<Encoder> _encoder;
//                    Codec * codec;
                    FrameConverter * _converter;
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
//                        ar & codec;
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
