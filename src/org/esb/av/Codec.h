#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H

#include "AV.h"

#include <boost/serialization/binary_object.hpp>
#include "Packet.h"
#include "org/esb/util/Log.h"

namespace org {
	namespace esb {
		namespace av {
			class Frame;
			class Codec{
			    logger("hive.av.codec")
			  public:
				const static int DECODER = 1;
				const static int ENCODER = 2;
				  Codec (const CodecID codecId, int mode = DECODER);
				  Codec (AVCodecContext * codec);
				  Codec ();
				 ~Codec ();
				CodecType getCodecType ();
				char *getCodecName ();
				int getCodecId ();
				void open ();
				void close ();
				void setWidth (int w);
				void setHeight (int h);
				void setPixelFormat (PixelFormat pfm);
				void setBitRate (int rate);
				void setTimeBase (AVRational tb);
				void setGopSize (int size);
				void setChannels (int size);
				void setSampleRate (int size);
				void setSampleFormat (SampleFormat size);
				void setFlag (int flag);
				int getWidth ();
				int getHeight ();
				int getPixelFormat ();
//				int getCodecType ();
//				string getCodecName ();

				  template < class Archive >
					void serialize (Archive & ar, const unsigned int version)
				{

					
					ar & _codec_id;
					ar & _mode;
					ar & _flags;
					ar & _pix_fmt;
					ar & _width;
					ar & _height;
					ar & _time_base.num;
					ar & _time_base.den;
					ar & _gop_size;
					ar & _bit_rate;
					ar & _channels;
					ar & _sample_rate;
					ar & _sample_format;

				}
				
				int _codec_id;
//			  protected:
				
				AVCodec * _codec;
				void findCodec (int mode);
				int _mode;
				AVCodecContext * ctx;
			protected:
				int 		_width;
				int 		_flags;
				int 		_height;
				PixelFormat _pix_fmt;
				int 		_bit_rate;
				AVRational 	_time_base;
				int 		_gop_size;
				int 		_channels;
				int 		_sample_rate;
				SampleFormat _sample_format;
				bool 		_opened;
				
			  private:
				void setParams ();
				
			};
}}}
#endif
