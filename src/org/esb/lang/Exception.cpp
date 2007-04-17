#include "Exception.h"


using namespace org::esb::lang;
Exception::Exception(){

}
Exception::Exception(const char * filename, const int linenumber, const char * message, ...){
    printf("Exception file:%s line %d Message %s", filename, linenumber, message);
}
