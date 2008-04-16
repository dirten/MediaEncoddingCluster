/******************************************************************************
* FOBS Video API test application 
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by Jos� San Pedro Wandelmer
*
*    This file is part of FOBS.
*
*    FOBS is free software; you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation; either version 2.1 
*    of the License, or (at your option) any later version.
*
*    FOBS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with FOBS; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "Decoder.h"
#include <time.h>
#include <iostream>
#include <unistd.h>

using namespace omnividea::fobs;
using namespace std;


int main(int argc, char *argv[])
{
    if(argc < 2) {
        cout << "Syntax: " << argv[0] << " inputfile" << endl;
    	return -1;
    }
    Decoder d(argv[1]);
    
    ReturnCode error = d.open();
    cout << "Video info: Width=" << d.getWidth() << " Height=" << d.getHeight() << " BitRate=" << d.getBitRate() << " FrameRate="<< d.getFrameRate() << endl;
    if(d.isAudioPresent())
      cout << "Audio info: SampleRate=" << d.getAudioSampleRate() << " BitRate=" << d.getAudioBitRate() << " Channels=" << d.getAudioChannelNumber() << endl;

    cout << "Loop begin...\n";
    error = d.nextFrame();
    cout << "First frame time: " << d.getFrameTime() << endl;
    if(argc > 2) error = d.setFrameByTime(10.0);
    clock_t time0 = clock();
    unsigned counter = 0;
    for(;isOk(error);)
    {
        //if(argc == 2) error = d.nextFrame();
        //else error = d.placeAtNextFrame();
        error = d.nextFrame();
        counter++;
    }
    if(isError(error)) cout << "Error: " << getErrorMessage(error)<< endl;
    clock_t timef = clock();
    d.close();
    cout << "Number of frames decoded: " << counter << endl;
    cout << "loop end...Time spent = " << ((timef-time0)/(double)CLOCKS_PER_SEC) << endl;
    return 0;

}