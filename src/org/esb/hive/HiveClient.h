/* 
 * File:   HiveClient.h
 * Author: jhoelscher
 *
 * Created on 29. Oktober 2008, 14:43
 */
#ifndef _HIVECLIENT_H
#define	_HIVECLIENT_H
#include "string"
#include "org/esb/net/TcpSocket.h"
//#include "org/esb/net/TcpSocket.h"

#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
//#include "org/esb/hive/job/ProcessUnit.h"
//#include "org/esb/util/Queue.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/Log.h"

#include "org/esb/lang/Ptr.h"
//#include "org/esb/io/QueueInputStream.h"
//#include "org/esb/io/QueueOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameConverter.h"

#include "../core/exports.h"
//#include <boost/interprocess/ipc/message_queue.hpp>
//namespace ipc=boost::interprocess;
namespace org {
    namespace esb {
      namespace net{
        //class TcpSocket;
      }
        namespace hive {

            class CORE_EXPORT HiveClient : public org::esb::signal::MessageListener {
              classlogger("org.esb.hive.HiveClient")
            public:
                HiveClient(std::string host, int port);
                // HiveClient(org::esb::net::TcpSocket & sock);
                ~HiveClient();
                void connect();
                void start();
                void stop();
                void process();
                void process2();
                void onMessage(org::esb::signal::Message&);
                void processUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit);
            private:
                boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit;
                bool _toHalt;
                bool _running;
                std::string _host;
                int _port;

                //Ptr<org::esb::io::QueueInputStream> _qis;
                //Ptr<org::esb::io::QueueOutputStream> _qos;
                Ptr<org::esb::io::ObjectInputStream> _ois;
                Ptr<org::esb::io::ObjectOutputStream> _oos;
                Ptr<org::esb::net::TcpSocket> _sock;
                //        org::esb::net::TcpSocket * _outsock;
                //		org::esb::util::Queue<boost::shared_ptr<job::ProcessUnit> ,10> inQueue;
                //		org::esb::util::Queue<boost::shared_ptr<job::ProcessUnit> ,10> outQueue;
                boost::mutex terminationMutex;
                boost::condition ctrlCHit;
                boost::thread _t;

                std::map<std::string, boost::shared_ptr<org::esb::av::Decoder> > _decoder_list;
                std::map<std::string, boost::shared_ptr<org::esb::av::Encoder> > _encoder_list;
                std::map<std::string, org::esb::av::FrameConverter *  > _converter_list;
                std::map<std::string, Ptr<org::esb::av::AVFilter>  > _filter_list;

                std::map<std::string, bool> _swap_codec_list;

//                boost::mutex thread_read_mutex;
//                boost::mutex thread_write_mutex;
                void packetReader();
                void packetWriter();
#ifdef USE_SAFMQ
                Ptr<org::esb::io::QueueInputStream> _qis;
                Ptr<org::esb::io::QueueOutputStream> _qos;
#endif
            };
        }
    }
}


#endif	/* _HIVECLIENT_H */

