#include "Codec.h"
extern "C" {
#include "avformat.h"
}
#include "Frame.h"
#include <iostream>
#include <assert.h>
#include "org/esb/util/Log.h"
using namespace std;
/*
void __attribute__ ((constructor)) my_init (void);

void my_init ()
{
	using namespace org::esb::av;
	cout << "test initialised"<<endl;
//	logdebug("init av package");
	av_register_all ();
	avcodec_register_all ();
}
*/
namespace org {
	namespace esb {
		namespace av {
			Codec::Codec () {
			    _opened=false;
			    	
					_codec_id=0;
					_mode=0;
					_pix_fmt=(PixelFormat)0;
					_width=0;
					_height=0;
					_time_base.num=0;
					_time_base.den=0;
					_gop_size=0;
					_bit_rate=0;
					_channels=0;
					_sample_rate=0;
					_sample_format=(SampleFormat)0;
			} 
			Codec::Codec (const CodecID codecId, int mode) {
				_codec_id = codecId;
				_codec = NULL;
				_mode = mode;
				if (_mode == DECODER) {
					_codec = avcodec_find_decoder ((CodecID)_codec_id);
				}
				else {
					_codec = avcodec_find_encoder ((CodecID)_codec_id);
				} if (_codec == NULL)
					logerror("Codec not found for id :" << codecId);
//					cout << "Codec not found for id :" << codecId << endl;
				avcodec_get_context_defaults2 (this, _codec->type);
	/*
					ar & _codec_id;
					ar & _mode;
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
	*/
				_width = 0;
				_height = 0;
				_bit_rate = 0;
				_gop_size = 0;
				_time_base.num=0;
				_time_base.den=0;
				_channels = 0;
				_sample_rate = 0;
				_sample_format = (SampleFormat)0;
				_pix_fmt = (PixelFormat)0;
    			    _opened=false;
			}

			CodecType Codec::getCodecType () {
				return codec_type;
			}

			char *Codec::getCodecName () {
				return codec_name;
			}

			int Codec::getCodecId () {
				return codec_id;
			}
/*
            AVCodecContext * Codec::getCodecContext(){
                return _codecCtx;
            }
*/
			void Codec::initDefaults () {
				avcodec_get_context_defaults (this	/*, CODEC_TYPE_UNKNOWN */
					);
			}

			void Codec::findCodec (int mode) {
			    logdebug("try to find "<<(mode==DECODER?"Decoder":"Encoder")<<" with id:"<<_codec_id);
				if (mode == DECODER) {
					_codec = avcodec_find_decoder ((CodecID)_codec_id);
					if (_codec == NULL)
					    logerror("Decoder not found for id :" << _codec_id);
				}
				else {
					_codec = avcodec_find_encoder ((CodecID)_codec_id);
					if (_codec == NULL)
					    logerror("Encoder not found for id :" << _codec_id);
				}
				avcodec_get_context_defaults2 (this, _codec->type);
				pix_fmt = _pix_fmt;
				width = _width;
				height = _height;
				bit_rate = _bit_rate;
				time_base = _time_base;
				gop_size = _gop_size;
				sample_rate = _sample_rate;
				sample_fmt = _sample_format;
				channels = _channels;
				max_b_frames=1;
			}

			void Codec::open () {
				findCodec (_mode);
				if (_codec->capabilities & CODEC_CAP_TRUNCATED)
					flags |= CODEC_FLAG_TRUNCATED;
				if (avcodec_open (this, _codec) < 0) {
					logerror("ERROR : while openning Codec" <<_codec_id);
//					cout << "ERROR : while openning Codec" <<avcodec_open (this, _codec) <<endl;
				}else{
				    logdebug("Codec opened:" << _codec_id);
				    _opened=true;
				}
			}
			Codec::~Codec () {
			    logdebug("Codec closed:" << _codec_id);
			    if(_opened){
					avcodec_close (this);
//					delete _codec;
				}
				_opened=false;
			}
			void Codec::setWidth (int w) {
				_width = w;
			}
			void Codec::setHeight (int h) {
				_height = h;
			}
			void Codec::setPixelFormat (PixelFormat pf) {
				_pix_fmt = pf;
			}
			void Codec::setBitRate (int rate) {
				_bit_rate = rate;
			}
			void Codec::setTimeBase (AVRational tb) {
				_time_base = tb;
			}
			void Codec::setGopSize (int size) {
				_gop_size = size;
			}
			void Codec::setChannels (int c) {
				_channels = c;
			}
			void Codec::setSampleRate (int rate) {
				_sample_rate = rate;
			}
			void Codec::setSampleFormat (SampleFormat f) {
				_sample_format = f;
			}

		}
	}
}
