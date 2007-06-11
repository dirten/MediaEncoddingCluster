#include "FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>


using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
FileInputStream::FileInputStream(File * file)throw (Exception) {
    const char * name=file!=NULL?file->getPath():NULL;
    if(!name) {
        throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
    }
    open(name);
}

FileInputStream::FileInputStream(const char * name)throw (Exception) {
    if(!name) {
        throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
    }
    open(name);
}

/**
 * Opens the specified file for reading.
 * @param name the name of the file
 */

void FileInputStream::open(const char * name)throw (Exception) {
    file=fopen(name,"r+");
    if(!file) {
        string error="FileInputStream::open - File not Found (";
        error+=name;
        error+=")";
        throw Exception(__FILE__, __LINE__, error.c_str());
    }
    
    fseek (file, 0, SEEK_END);
    _filePointer= ftell(file);
    fseek (file, 0, 0);
    
}

void FileInputStream::close() {
    fclose(file);
}

int FileInputStream::available(bool isBlocking) {
	return _filePointer; 
}

int FileInputStream::read(unsigned char * buffer, int length) {
    int read=fread((char*)buffer,1, length, file);
	_filePointer-=read;
    return read;
}


FileInputStream::~FileInputStream() {
    this->close();
}
