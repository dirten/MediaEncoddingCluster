/* 
 * File:   Process.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:45
 */

#ifndef PROCESS_H
#define	PROCESS_H
#include <string>
#include <list>
#include <inttypes.h>
namespace org {
  namespace esb {
    namespace lang {

      class Process {
      public:
        Process(std::string e, std::list<std::string> args=std::list<std::string>());
        Process(const Process& orig);
        virtual ~Process();
        void start();
        void run(bool restartable=false);
        void stop();
        void kill();
        bool isRunning();
      private:
        std::string _executable;
        std::list<std::string> _arguments;
        int32_t _processId;
        bool _running;
        bool _restartable;
      };
    }
  }
}

#endif	/* PROCESS_H */

