/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/
#include "FrameContainer.h"
#include <string>
using namespace std;

int frameCounter=0;
int pathCounter=0;
string pfad=(string)"/tmp";
FILE *pFileHive;
FrameCompressor *compressor;
/***************************************************************************/
FrameContainer::FrameContainer(string path){
    pfad=path;
    pFileHive=fopen("/tmp/frame.container/hive.data", "wb");

    compressor=new FrameCompressor();

 
}


/***************************************************************************/
void FrameContainer::putFrame2(AVFrame * frame, AVCodecContext *codecCtx ){
  for(int y=0; y<codecCtx->height; y++)
    fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFileHive);
//    fflush(pFileHive);
}

void FrameContainer::putFrame4(AVFrame * frame, AVCodecContext *codecCtx ){
    unsigned char * data=new unsigned char[512*256*3];
    int filelen=compressor->deflateFrame(frame, data);

//  for(int y=0; y<codecCtx->height; y++)
    fwrite(data, 1, filelen, pFileHive);
    delete data;
}
/***************************************************************************/
void FrameContainer::putFrame3(AVFrame * frame, AVCodecContext *codecCtx ){
    unsigned char * data=new unsigned char[512*256*3];
    bzero(data, 512*256*3);
//    FrameCompressor *compressor=new FrameCompressor();
    int filelen=compressor->deflateFrame(frame, data);
//    cout << "Data:"<<(string((const char*)data).length())<<endl;



  FILE *pFile;
  char szFilename[64];
    if(frameCounter%1000==0){
	++pathCounter;
	char newPfad[64];
	sprintf(newPfad, "%s/%d",pfad.c_str(), pathCounter);
	mkdir(newPfad,0755);
    }
  sprintf(szFilename, "%s/%d/frame%d.data3.ppm",pfad.c_str(),pathCounter, ++frameCounter);
//  cout <<szFilename<<endl;
  pFile=fopen(szFilename, "wb");
//  fprintf(pFile, "P6\n%d %d\n255\n", 512, 256);
//  for(int y=0; y<256; y++)
    fwrite(data, 1, filelen, pFile);
  fclose(pFile);
  delete data;
}

void FrameContainer::putFrame(AVFrame * frame, AVCodecContext *codecCtx ){
  FILE *pFile;
  BZFILE* b;
  int bufSize=codecCtx->width*3;
  char buf[bufSize];
  int bzerror;
  int     nWritten;
  unsigned int nBytesIn;
  unsigned int nBytesOut;

  char szFilename[64];
  int  y;
    // Open file
    if(frameCounter%1000==0){
	++pathCounter;
	char newPfad[64];
	sprintf(newPfad, "%s/%d",pfad.c_str(), pathCounter);
	mkdir(newPfad,0755);
    }
  sprintf(szFilename, "%s/%d/frame%d.data.ppm",pfad.c_str(),pathCounter, ++frameCounter);
//  cout <<szFilename<<endl;
  pFile=fopen(szFilename, "wb");

        
/*
  b = BZ2_bzWriteOpen( &bzerror, pFile, 9 ,0,250);
//  while ( ) {
  for(y=0; y<codecCtx->height; y++){
    //nWritten = 
    BZ2_bzWrite ( &bzerror, b, frame->data[0]+y*frame->linesize[0], bufSize );
    if (bzerror == BZ_IO_ERROR) { 
      BZ2_bzWriteClose ( &bzerror, b, 0, &nBytesIn, &nBytesOut );
	cout << "Fehler beim schreiben von bzFrame"<<frameCounter<<endl;
    }
  }

  BZ2_bzWriteClose ( &bzerror, b, 0, &nBytesIn, &nBytesOut );
  if (bzerror == BZ_IO_ERROR) {
	cout << "Fehler beim schliessen von bzFrame"<<frameCounter<<endl;    
  }
*/

  if(pFile==NULL)
    return;

  fprintf(pFile, "P6\n%d %d\n255\n", codecCtx->width, codecCtx->height);
  for(y=0; y<codecCtx->height; y++)
    fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFile);
  fclose(pFile);


}
/***************************************************************************/
AVFrame * FrameContainer::getFrame(){


}

