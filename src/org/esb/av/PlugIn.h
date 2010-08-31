/* 
 * File:   PlugIn.h
 * Author: HoelscJ
 *
 * Created on 30. August 2010, 13:10
 */

#ifndef PLUGIN_H
#define	PLUGIN_H
#include <string>
namespace org {
  namespace esb {
    namespace av {
      class PlugIn {
      public:
        enum ProcessStatus{
          BUFFER_PROCESSED_FAILED =-1,
          BUFFER_PROCESSED_OK ,
          INPUT_BUFFER_NOT_CONSUMED ,
          OUTPUT_BUFFER_NOT_FILLED ,
          PLUGIN_TERMINATED
        };

        virtual void close() = 0;
        virtual std::string getName() = 0;
        virtual void open() = 0;
        virtual void reset() = 0;
      };
    }
  }
}

#endif	/* PLUGIN_H */

