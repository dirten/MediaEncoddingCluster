/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/

#ifndef FRAMECONTAINER_H
#include <avformat.h>
#include <iostream>
#include <sqlite3.h>
#include <sys/stat.h>
#include <bzlib.h>
#include "FrameCompressor.h"
#define FRAMECONTAINER_H
using namespace std;
class FrameContainer {
    public:
	FrameContainer(string path);
	void putFrame( AVFrame * frame, AVCodecContext *codexCtx );
	void putFrame2( AVFrame * frame, AVCodecContext *codexCtx );
	void putFrame3( AVFrame * frame, AVCodecContext *codexCtx );
	void putFrame4( AVFrame * frame, AVCodecContext *codexCtx );
	AVFrame * getFrame();
    private:
};
#endif 
