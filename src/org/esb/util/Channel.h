/* 
 * File:   Channel.h
 * Author: HoelscJ
 *
 * Created on 25. Februar 2011, 10:48
 */

#ifndef CHANNEL_H
#define	CHANNEL_H
#include "Queue.h"
namespace org {
  namespace esb {
    namespace util {

      template <typename T>
      class Channel:public Queue<T, 1> {
      private:
        bool _closed;

      public:
        Channel(){
          _closed=false;
        }
        virtual ~Channel() {
         std::cout << " ~Channel()"<<std::endl;

          //Queue<T, size>::flush();
        }
        void close(){
          this->flush();
          _closed=true;
        }

        bool operator<<(T data)
        {
          if(_closed)return false;
          return enqueue(data);
            //return true;
        }

        bool operator>>(  T & data)
        {
          if(_closed)return false;
          return dequeue(data);
          //return true;
        }

        bool isClosed(){
          return _closed;
        }
      };
      //typedef Queue<typename, int> Channel;

    }
  }
}
#endif	/* CHANNEL_H */

