/* 
 * File:   FrameProcessor.h
 * Author: HoelscJ
 *
 * Created on 6. August 2010, 16:37
 */

#ifndef FRAMEPROCESSOR_H
#define	FRAMEPROCESSOR_H
#include "Frame.h"

#include "Sink.h"
namespace org {
  namespace esb {
    namespace av {


class FrameProcessor {
      public:
        FrameProcessor();
        FrameProcessor(const FrameProcessor& orig);
        virtual ~FrameProcessor();
        virtual void process(Frame & input, Frame & output);
        virtual void setInputSink(Sink & sink);
      private:

      };
    }
  }
}

#endif	/* FRAMEPROCESSOR_H */

