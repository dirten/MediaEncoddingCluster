/* 
 * File:   ProcessManager.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:42
 */

#ifndef PROCESSMANAGER_H
#define	PROCESSMANAGER_H
namespace org {
  namespace esb {
    namespace hive {

      class ProcessManager {
      public:
        ProcessManager();
        ProcessManager(const ProcessManager& orig);
        virtual ~ProcessManager();
      private:

      };
    }
  }
}

#endif	/* PROCESSMANAGER_H */

