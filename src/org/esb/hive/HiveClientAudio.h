/* 
 * File:   HiveClient.h
 * Author: jhoelscher
 *
 * Created on 29. Oktober 2008, 14:43
 */
#ifndef _HIVECLIENTAUDIO_H
#define	_HIVECLIENTAUDIO_H
#include "org/esb/net/TcpSocket.h"
#include "string"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameConverter.h"
//#include "org/esb/hive/job/ProcessUnit.h"
//#include "org/esb/util/Queue.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "org/esb/util/Log.h"
#include "exports.h"
//#include <boost/interprocess/ipc/message_queue.hpp>
//namespace ipc=boost::interprocess;
namespace org {
    namespace esb {
        namespace hive {

            class HIVE_EXPORT HiveClientAudio : public org::esb::signal::MessageListener {
              classlogger("org.esb.hive.HiveClientAudio")
            public:
                HiveClientAudio(std::string host, int port);
                // HiveClient(org::esb::net::TcpSocket & sock);
                ~HiveClientAudio();
                void connect();
                void start();
                void stop();
                void process();
                void process2();
                void onMessage(org::esb::signal::Message&);
            private:
                bool _toHalt;
                bool _running;
                bool _swap_codecs;
                std::string _host;
                int _port;
                org::esb::io::ObjectInputStream * _ois;
                org::esb::io::ObjectOutputStream * _oos;
                org::esb::net::TcpSocket * _sock;
                //        org::esb::net::TcpSocket * _outsock;
                //		org::esb::util::Queue<boost::shared_ptr<job::ProcessUnit> ,10> inQueue;
                //		org::esb::util::Queue<boost::shared_ptr<job::ProcessUnit> ,10> outQueue;
                boost::mutex terminationMutex;
                boost::condition ctrlCHit;
//                boost::mutex thread_read_mutex;
//                boost::mutex thread_write_mutex;
                void packetReader();
                void packetWriter();
                boost::shared_ptr<org::esb::av::Decoder> _dec;
                boost::shared_ptr<org::esb::av::Encoder> _enc;
                std::map<int, boost::shared_ptr<org::esb::av::Decoder> > _decoder_list;
                std::map<int, boost::shared_ptr<org::esb::av::Encoder> > _encoder_list;
                std::map<int, org::esb::av::FrameConverter *  > _converter_list;
                std::map<int, bool> _swap_codec_list;
                org::esb::av::FrameConverter * _conv;
            };
        }
    }
}


#endif	/* _HIVECLIENT_H */

