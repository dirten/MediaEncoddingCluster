/* 
 * File:   HookProvider.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 16:47
 */

#ifndef HOOKPROVIDER_H
#define	HOOKPROVIDER_H
namespace org {
  namespace esb {
    namespace core {

      class HookProvider{
      public:
        void getHookNotificationCenter();
        virtual ~HookProvider(){};
      private:

      };
    }
  }
}


#endif	/* HOOKPROVIDER_H */

