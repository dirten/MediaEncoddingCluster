/* 
 * File:   Object.h
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:19
 */

#ifndef OBJECT_H
#define	OBJECT_H
#include "boost/intrusive_ptr.hpp"
namespace org {
  namespace esb {
    namespace lang {
      

      template <typename T=std::string>
      class Object : public boost::intrusive_ptr<T> {
      public:

        Object() : references(0) {
        }
        virtual ~Object();
      private:
        long references;

        void intrusive_ptr_add_ref(){

        }

      };
        void intrusive_ptr_release(Object<> * p){

        }
    }
  }
}

#endif	/* OBJECT_H */

