/* 
 * File:   TestPreviewFrameServer.cpp
 * Author: HoelscJ
 *
 * Created on 9. August 2010, 12:52
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/av/Frame.h"
#include "org/esb/web/project/PreviewFrameServer.h"


#include "org/esb/lang/Ptr.h"
using namespace std;
using namespace org::esb::web;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  PreviewFrameServer server;
  std::string file_path = MEC_SOURCE_DIR;
  file_path+="/test.dvd";
  server.setFileName(file_path);
  for(int a=0;a<5;a++){
    Ptr<Frame> frame=server.getFrame();
    assert(a==frame->getPts());
    server.jumpForward(PreviewFrameServer::JUMP_ONE);
  }
  
  for(int a=0;a<5;a++){
    Ptr<Frame> frame=server.getFrame();
//    assert(a==frame->getPts());
    server.jumpPrevious(PreviewFrameServer::JUMP_ONE);
  }

  Log::close();
  return 0;
}

