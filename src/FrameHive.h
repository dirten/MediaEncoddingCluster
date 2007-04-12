/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/

#ifndef FRAMEHIVE_H
#include <avformat.h>
#include <iostream>
#include <sqlite3.h>

#define FRAMEHIVE_H
using namespace std;
class FrameHive {
    public:
	FrameHive(string db);
	void putFrame( AVFrame * frame, AVCodecContext *codexCtx );
	AVFrame * getFrame();
	~FrameHive();
    private:
    
};
#endif 
