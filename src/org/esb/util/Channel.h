/* 
 * File:   Channel.h
 * Author: HoelscJ
 *
 * Created on 25. Februar 2011, 10:48
 */

#ifndef CHANNEL_H
#define	CHANNEL_H
#include "Queue.h"
#if defined(_WIN32)
  #if defined(util_EXPORTS)
    #define UTIL_EXPORT __declspec( dllexport )
  #else
    #define UTIL_EXPORT __declspec( dllimport )
  #endif
#else
  #define UTIL_EXPORT
#endif
namespace org {
  namespace esb {
    namespace util {
      template <typename T>
      class UTIL_EXPORT Channel:public Queue<T, 1> {
      public:
        Channel(){
        }
        virtual ~Channel() {
         std::cout << " ~Channel()"<<std::endl;
        }
        void close(){
          this->flush();
        }

        bool operator<<(T data)
        {
          return enqueue(data);
        }

        bool operator>>(  T & data)
        {
          return dequeue(data);
        }

        bool isClosed(){
          return Queue<T, 1>::closed();
        }
      };
    }
  }
}
#endif	/* CHANNEL_H */

