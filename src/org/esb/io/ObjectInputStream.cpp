#include "ObjectInputStream.h"

using namespace org::esb::io;
		ObjectInputStream::ObjectInputStream(InputStream * is):_is(is){};
		int ObjectInputStream::available(bool isBlocking){return 0;};
		int ObjectInputStream::read(unsigned char * buffer, int length){return 0;};
		int ObjectInputStream::read(vector<unsigned char>&buffer){return 0;};
		int ObjectInputStream::read(){return 0;};
