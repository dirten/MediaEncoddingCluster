#include "StringOutputStream.h"


namespace org {
  namespace esb {
    namespace io {
      StringOutputStream::StringOutputStream(std::string & data){
        _data=data;
      }
      void StringOutputStream::write(char * buffer, int length) {
        _data.append(buffer, length);
      }

      void StringOutputStream::write(string & str) {
        _data.append(str);
      }
      std::string StringOutputStream::getData(){
        return _data;
      }
    }
  }
}
