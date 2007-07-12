#include "FrameHive.h"

#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace activemq::core;
using namespace org::esb::config;


/*****************************************************************************/
FrameHive::FrameHive(string dbname)
{

         // Create a ConnectionFactory
    std::string brokerURI =
        "tcp://127.0.0.1:61616"
        "?wireFormat=openwire";

            ActiveMQConnectionFactory* connectionFactory =
                new ActiveMQConnectionFactory( brokerURI );




            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();

            // free the factory, we are done with it.

            delete connectionFactory;

            // Create a Session
            session = connection->createSession( Session::AUTO_ACKNOWLEDGE );

            // Create the destination (Topic or Queue)
            destination = session->createQueue( "TEST.FOO" );

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );






}
/*****************************************************************************/
FrameHive::~FrameHive()
{

}

/*****************************************************************************/
void FrameHive::putFrame( AVFrame * frame, AVCodecContext *codecCtx )
{
  //    this->putFrameHive(frame, codecCtx);
  //    this->putFrameSQLite(frame, codecCtx);
  //    this->putFrameMySQL(frame, codecCtx);
  this->putFrameFS(frame, codecCtx);
}
/*****************************************************************************/
void FrameHive::putPacket( AVPacket * packet )
{
                BytesMessage* message = session->createBytesMessage( packet->data, packet->size);
//                printf( "Sent message #%d from thread %s\n", ix, threadIdStr.c_str() );
                producer->send( message );

                delete message;

}
/*****************************************************************************/
void FrameHive::putFrameHive( AVFrame * frame, AVCodecContext *codecCtx )
{
  if(COMPRESSED)
  {
    unsigned char * data=new unsigned char[512*256*3];
    int filelen=compressor->deflateFrame(frame, data);
    fwrite(data, 1, filelen, pFileHive);
    delete data;
  }
  else
  {
    for(int y=0; y<codecCtx->height; y++)
      fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFileHive);
    fflush(pFileHive);
  }
}

/*****************************************************************************/
void FrameHive::putFrameFS( AVFrame * frame, AVCodecContext *codecCtx )
{
  FILE *pFile;
  int bufSize=codecCtx->width*3;

  char szFilename[64];
  int  y;
  // Open file
  if(this->frameCounter%1000==0)
  {
    ++this->pathCounter;
    char newPfad[64];
    sprintf(newPfad, "%s/%d",pfad.c_str(), this->pathCounter);
    mkdir(newPfad,0755);
  }
  sprintf(szFilename, "%s/%d/frame%d.fstream.ppm",pfad.c_str(),pathCounter, ++frameCounter);
  //  cout <<szFilename<<endl;

  pFile=fopen(szFilename, "w+b");
  if(pFile==NULL)
    return;
  fprintf(pFile, "P6\n%d %d\n255\n", codecCtx->width, codecCtx->height);
  for(y=0; y<codecCtx->height; y++)
    fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFile);
  fclose(pFile);

  /*
      string header="P6\n";
      header+="512 256";
  //    header+="\n";
  //    header+="256";
      header+="\n";
      header+="255\n";
      fstream file(szFilename, ios::out|ios::binary);
      file.write(header.c_str(), header.size());
      file.write((const char*)frame->data[0], (codecCtx->width*codecCtx->width*3));
      file.close();
  */
}


