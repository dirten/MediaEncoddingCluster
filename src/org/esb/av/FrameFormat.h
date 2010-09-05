#ifndef ORG_ESB_AV_FRAMEFORMAT_H
#define ORG_ESB_AV_FRAMEFORMAT_H

#include "AV.h"


namespace org{
namespace esb{
namespace av{
struct FrameFormat{
	PixelFormat pixel_format;
	int width;
	int height;
	int channels;
	int samplerate;
  SampleFormat sampleformat;
	/*
	FrameFormat(){
		pixel_format=(PixelFormat)0;
		int width=0;
		int height=0;
		int channels=0;
		int samplerate=0;
	}*/
};
}}}
#endif
