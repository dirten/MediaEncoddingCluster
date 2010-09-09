/* 
 * File:   PreviewFrameServer.cpp
 * Author: HoelscJ
 *
 * Created on 7. September 2010, 14:21
 */

#include <cstdlib>

#include "org/esb/web/project/PreviewFrameServer.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::web;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path=MEC_SOURCE_DIR;

  PreviewFrameServer server;
  server.setFileName(path+"/test.dvd");
  Ptr<Frame> frame=server.getFrame();
  LOGDEBUG("!!!!!!!!!!!!!!!!"<<frame->toString());
  server.jumpForward(PreviewFrameServer::JUMP_TEN);
  frame=server.getFrame();
  LOGDEBUG("!!!!!!!!!!!!!!!!"<<frame->toString());
  server.jumpForward(PreviewFrameServer::JUMP_TEN);
  frame=server.getFrame();
  LOGDEBUG("!!!!!!!!!!!!!!!!"<<frame->toString());
  server.jumpForward(PreviewFrameServer::JUMP_TEN);
  frame=server.getFrame();
  LOGDEBUG("!!!!!!!!!!!!!!!!"<<frame->toString());

  server.jumpPrevious(PreviewFrameServer::JUMP_TEN);
  frame=server.getFrame();
  LOGDEBUG("!!!!!!!!!!!!!!!!"<<frame->toString());

  Ptr<Decoder> decoder=server.getDecoder();
  Log::close();
  return 0;
}

