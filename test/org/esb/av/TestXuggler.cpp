/* 
 * File:   TestXuggler.cpp
 * Author: jhoelscher
 *
 * Created on 28. Januar 2010, 11:00
 */

#include <stdlib.h>
#include "org/esb/util/Log.h"

#include "com/xuggle/xuggler/IContainer.h"
//#include "com/xuggle/xuggler/Container.h"
//#include "com/xuggle/xuggler/Global.h"
#include "com/xuggle/xuggler/IStream.h"
#include "com/xuggle/xuggler/IRational.h"
#include "com/xuggle/xuggler/IError.h"
#include "org/esb/av/AV.h"
/*
 * 
 */

using namespace com::xuggle::xuggler;
using namespace com::xuggle::ferry;

int main(int argc, char** argv) {
  Log::open();
  com::xuggle::xuggler::IContainer *container = com::xuggle::xuggler::IContainer::make();
  av_log_set_level(AV_LOG_DEBUG);

  std::string path = MEC_SOURCE_DIR;
  int ret = container->open(path.append("/test/images/number-%d.pgm").c_str(), IContainer::READ, NULL);
  if (ret < 0) {
    LOGERROR("org.esb.av.TestXuggler", "Failure opening " << path)
    LOGDEBUG("org.esb.av.TestXuggler", "ErrorCode:" << IError::make(ret)->getDescription());
    exit(1);
  }
  int numStreams = container->getNumStreams();

  for (int i = 0; i < numStreams; i++) {
    IStream *stream = container->getStream(i);
    LOGDEBUG("org.esb.av.TestXuggler","Stream:"<<stream->getFrameRate()->getNumerator()<<"/"<<stream->getFrameRate()->getDenominator());
  }
  Log::close();

  return (EXIT_SUCCESS);
}

