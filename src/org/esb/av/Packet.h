#ifndef ORG_ESB_PACKET_H
#define ORG_ESB_PACKET_H
extern "C" {
#include "avformat.h"
}
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/serialization/split_member.hpp>
//using namespace boost;
namespace org{
    namespace esb{
	namespace av{
	    class Packet:public AVPacket
	    {
		public:
		    Packet();
		    Packet(int size);
//		    Packet(Packet * packet);
		    ~Packet();
        	    uint8_t * getData();
    		    int getSize();
        	    int getPts();
        	    int getDts();
        	    int getFlags();
        	    int getStreamIndex();
        	    int getDuration();
		    bool isKeyFrame();
        	    void * getPriv();
        	    int64_t getPosition();
//        	    boost::shared_ptr<unsigned char*> data;
    		private:
        	    bool isCopy;
        	    bool callDestruct;
        	    uint8_t _data;
        	    friend class boost::serialization::access;
//		    void serialize(boost::archive::Archive & ar, const unsigned int version);

		    template<class Archive>
		        void serialize(Archive & ar, const unsigned int version)
		            {
		                ar & size;
				if(data==NULL){
				    data=new uint8_t[size];
				    memset(data,0,size);
				    isCopy=true;
				}
				ar & boost::serialization::make_binary_object(data,size);
		                ar & pts;
		                ar & dts;
		                ar & flags;
		                ar & stream_index;
		                ar & duration;
		                ar & pos;
		            }
		            
	    };
	}
    }
}
#endif
