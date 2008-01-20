#include "ObjectInputStream.h"

using namespace org::esb::io;
		ObjectInputStream::ObjectInputStream(InputStream * is){_is=is;};
		int ObjectInputStream::available(bool isBlocking){};
		int ObjectInputStream::read(unsigned char * buffer, int length){};
		int ObjectInputStream::read(vector<unsigned char>&buffer){};
		int ObjectInputStream::read(){};
