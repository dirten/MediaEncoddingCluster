#include "boost/shared_ptr.hpp"

#ifndef ORG_ESB_LANG_PTR
#define ORG_ESB_LANG_PTR

template <typename T>
class Ptr:public boost::shared_ptr<T>{
public:
  Ptr(){}
  Ptr(T * elem):boost::shared_ptr<T>(elem){
  }
};
#endif

