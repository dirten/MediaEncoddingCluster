/* 
 * File:   CtrlCHitWaiter.h
 * Author: HoelscJ
 *
 * Created on 6. August 2010, 12:56
 */

#ifndef CTRLCHITWAITER_H
#define	CTRLCHITWAITER_H
namespace org {
  namespace esb {
    namespace lang {

      class CtrlCHitWaiter {
      public:
        static void wait();
      private:
        CtrlCHitWaiter();
        ~CtrlCHitWaiter();

      };
    }
  }
}

#endif	/* CTRLCHITWAITER_H */

