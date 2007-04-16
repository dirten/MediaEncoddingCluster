/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/

#ifndef FRAMEHIVE_H
#define _FILE_OFFSET_BITS 64  /* enable large file support  */
#include <avformat.h>
#include <iostream>
#include <sqlite3.h>
#include <mysql/mysql.h>
#include <sys/stat.h>
#include "FrameCompressor.h"
#include "org/esb/config/config.h"
#include <fstream>
#define FRAMEHIVE_H
#define COMPRESSED false
using namespace std;
class FrameHive {
    public:
	FrameHive(string db);
	void putFrame( AVFrame * frame, AVCodecContext *codexCtx );
	AVFrame * getFrame();
	~FrameHive();
    private:
	void putFrameSQLite( AVFrame * frame, AVCodecContext *codexCtx );
	void putFrameMySQL( AVFrame * frame, AVCodecContext *codexCtx );
	void putFrameFS( AVFrame * frame, AVCodecContext *codexCtx );    
	void putFrameHive( AVFrame * frame, AVCodecContext *codexCtx );    
	int frameCounter;
	int pathCounter;
	string pfad;
	FrameCompressor *compressor;
	FILE *pFileHive;
};
#endif 
