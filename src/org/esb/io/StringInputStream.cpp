#include "StringInputStream.h"

namespace org {
  namespace esb {
    namespace io {

      StringInputStream::StringInputStream(std::string & data) {
        _data = data;
        _position=0;
      }

      int StringInputStream::read(unsigned char * buffer, int length) {
        std::cout << _data<<std::endl;
        length=min((size_t)length,_data.length());
        std::string sub=_data.substr(_position,length);
        _position+=length;
        const char * tmp=sub.c_str();
        memcpy(buffer,tmp,length);
        return length;
      }

      int StringInputStream::read(vector<unsigned char>&buffer) {

      }

      int StringInputStream::read(string & str) {
        str = _data.substr(_position,_data.length());
      }

      int StringInputStream::read() {

      }
      long long int StringInputStream::available(bool blocked){
        return _data.length();
      }

    }
  }
}
