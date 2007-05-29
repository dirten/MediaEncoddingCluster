/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <avcodec.h>
#include <avformat.h>

#include <stdio.h>
//#include "framesender.h"
//#include "FrameContainer.h"
#include "FrameHive.h"
#include "org/esb/config/config.h"
using namespace std;
using namespace org::esb::config;

bool GetNextFrame(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, 
                  int videoStream, AVFrame *pFrame)
{
  static AVPacket packet;
  static int      bytesRemaining=0;
  static uint8_t  *rawData;
  static bool     fFirstTime=true;
  int             bytesDecoded;
  int             frameFinished;

    // First time we're called, set packet.data to NULL to indicate it
    // doesn't have to be freed
  if(fFirstTime)
  {
    fFirstTime=false;
    packet.data=NULL;
  }

    // Decode packets until we have decoded a complete frame
  while(true)
  {
        // Work on the current packet until we have decoded all of it
    while(bytesRemaining > 0)
    {
//     cout << "Laenge : "<< strlen((const char *) rawData) << endl;

            // Decode the next chunk of data
      
      bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame,
                                        &frameFinished, rawData, bytesRemaining);
     

            // Was there an error?
    
      if(bytesDecoded < 0)
      {
        fprintf(stderr, "Error while decoding frame\n");
//        return true;
      }

      bytesRemaining-=bytesDecoded;
      rawData+=bytesDecoded;

            // Did we finish the current frame? Then we can return
      if(frameFinished){
        return true;
      }
    }

        // Read the next packet, skipping all packets that aren't for this
        // stream
    do
    {
            // Free old packet
      if(packet.data!=NULL)
        av_free_packet(&packet);

            // Read new packet
      if(av_read_packet(pFormatCtx, &packet)<0)
        goto loop_exit;
    } while(packet.stream_index!=videoStream);

    bytesRemaining=packet.size;

//    cout << "Packet size: "<<packet.size<<endl;
    rawData=packet.data;
  }

loop_exit:

    // Decode the rest of the last frame
    bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame, &frameFinished, 
                                      rawData, bytesRemaining);

    // Free last packet
    if(packet.data!=NULL)
      av_free_packet(&packet);

    return frameFinished!=0;
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
  FILE *pFile;
  char szFilename[32];
  int  y;

    // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

    // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
  fclose(pFile);
}









int main(int argc, char *argv[])
{
    Config::init("cluster.cfg");


    //============================================================
    // set to true to use topics instead of queues
    // Note in the code above that this causes createTopic or
    // createQueue to be used in both consumer an producer.
    //============================================================    
    bool useTopics = false;  
/*
    HelloWorldProducer producer( 1000, useTopics );
    
    // Start the producer thread.
    Thread producerThread( &producer );
    producerThread.start();

    // Wait for the threads to complete.
    producerThread.join();
*/
    std::cout << "-----------------------------------------------------\n";    
    std::cout << "Finished with the example, ignore errors from this" 
              << std::endl
              << "point on as the sockets breaks when we shutdown."
              << std::endl;
    std::cout << "=====================================================\n";    




  AVFormatContext *pFormatCtx;
  int             i, videoStream;
  AVCodecContext  *pCodecCtx;
  AVCodec         *pCodec;
  AVFrame         *pFrame; 
  AVFrame         *pFrameRGB;
  int             numBytes;
  uint8_t         *buffer;

    // Register all formats and codecs
  av_register_all();

    
//  argv[1]="/media/video/Sledge Hammer/Sledge_Staffel1_episode1.avi";
//  argv[1]="/home/jhoelscher/bripper/Der Blutige Pfad Gottes - German (DVD-Quali).avi";
    string path=string(Config::getProperty("src.path"));
    path+="/";
    path+="Der Blutige Pfad Gottes - German (DVD-Quali).avi";
    argv[1]=(char*)path.c_str();

  // Open video file
  if(av_open_input_file(&pFormatCtx, argv[1], NULL, 0, NULL)!=0){
    cout << "Konnte Datei " << argv[1] << " nicht oeffnen" <<endl;
    return -1; // Couldn't open file
  }
    // Retrieve stream information
  if(av_find_stream_info(pFormatCtx)<0){
    cout << "Konnte StreamInfo von " << argv[1] << " nicht ermitteln" <<endl;
    return -1; // Couldn't find stream information
  }
  
  dump_format(pFormatCtx, 0, argv[1], false);
  
  // Find the first video stream
  videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++){
    AVCodecContext *cctx=pFormatCtx->streams[i]->codec;
    if(cctx->codec_type==CODEC_TYPE_VIDEO)
    {
      videoStream=i;
      break;
    }
  }
//    av_seek_frame(pFormatCtx,videoStream,17000,AVSEEK_FLAG_ANY);  
  if(videoStream==-1)
    return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
  AVStream *str=pFormatCtx->streams[videoStream];
//  pCodecCtx=pFormatCtx->streams[videoStream]->codec;
  pCodecCtx=str->codec;
//  char * text;
//  sprintf(text,"%5.2f", av_q2d(str->r_frame_rate));
//  cout << "FrameRate "<< text<< endl;
  cout << "Stream "<<av_q2d(str->r_frame_rate)<< endl;
  cout << "StreamId "<<videoStream<< endl;
  cout << "CodecId "<<pCodecCtx->codec_id<< endl;
  cout << "CodecName "<<pCodecCtx->codec_name<< endl;
//  pCodecCtx=pFormatCtx->streams[videoStream]->codec;
//  delete text;
      // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL)
    return -1; // Codec not found

    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
  if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
    pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;

    // Open codec
  if(avcodec_open(pCodecCtx, pCodec)<0)
    return -1; // Could not open codec

    // Hack to correct wrong frame rates that seem to be generated by some 
    // codecs
  /*
  if(pCodecCtx->frame_rate>1000 && pCodecCtx->frame_rate_base==1)
    pCodecCtx->frame_rate_base=1000;
  */
    // Allocate video frame
  pFrame=avcodec_alloc_frame();

    // Allocate an AVFrame structure
  pFrameRGB=avcodec_alloc_frame();
  if(pFrameRGB==NULL)
    return -1;

    // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                              pCodecCtx->height);
  buffer=new uint8_t[numBytes];

    // Assign appropriate parts of buffer to image planes in pFrameRGB
  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                  pCodecCtx->width, pCodecCtx->height);

  
   // Read frames and save first five frames to disk
  i=0;
//  FrameContainer *container=new FrameContainer("/tmp/frame.container");
//  FrameContainer *container=new FrameContainer("/media/video/test");
//  FrameContainer *container=new FrameContainer("/tmp/frame.container");
  FrameHive *hive=new FrameHive("test.db");
  while(GetNextFrame(pFormatCtx, pCodecCtx, videoStream, pFrame)&&i<500)
  {
    img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, (AVPicture*)pFrame, 
                 pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    

    if(i%10==0){
	cerr << "\rProcessing Frame :"<< i;
    }
        // Save the frame to disk
    ++i;
//    if(i<=5)
//    cout << "Laenge : "<< strlen((const char *) pFrame->data[0]) << endl;
////  SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
//    sender->send((const char *)pFrame->data[0]);
//    container->putFrame(pFrameRGB,pCodecCtx);
//    container->putFrame3(pFrameRGB,pCodecCtx);
//    container->putFrame(pFrameRGB,pCodecCtx);
//      hive->putFrame(pFrameRGB,pCodecCtx);
//    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
  }
	cerr << "\rProcessing Frame :"<< i;
  cout << endl;
//    delete container;
    // Free the RGB image
  delete [] buffer;
  av_free(pFrameRGB);

    // Free the YUV frame
  av_free(pFrame);

    // Close the codec
  avcodec_close(pCodecCtx);

    // Close the video file
  av_close_input_file(pFormatCtx);

  return 0;
  cout << "Hello, world!" << endl;

  return EXIT_SUCCESS;
}
