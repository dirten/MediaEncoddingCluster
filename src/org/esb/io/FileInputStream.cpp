#include "FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>


using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
FileInputStream::FileInputStream(File * file){
    const std::string name=file!=NULL?file->getPath():NULL;
    if(!(name.size()>0)) {
//        throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
    }
    open(name);
}

FileInputStream::FileInputStream(const std::string name) {
    if(!(name.size()>0)) {
//        throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
    }
    open(name);
}

/**
 * Opens the specified file for reading.
 * @param name the name of the file
 */

void FileInputStream::open(const std::string name){
    file=fopen(name.c_str(),"r+");
    if(!file) {
        string error="FileInputStream::open - File not Found (";
        error+=name;
        error+=")";

//        throw Exception(__FILE__, __LINE__, error.c_str());
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

int FileInputStream::read(string & str) {
    int size=available();
	char * buffer=new char[size];
    int read=fread((char*)buffer,1, size, file);
    _filePointer-=read;
	str=string(buffer,size);
	delete []buffer;
    return read;
}
int FileInputStream::read(unsigned char * buffer, int length) {
    int read=fread((char*)buffer,1, length, file);
    _filePointer-=read;
    return read;
}

int FileInputStream::read() {
	read((unsigned char *)&_byte,1);
    return _byte;
}

int FileInputStream::read(vector<unsigned char>&buffer) {
    size_t size=buffer.size();
    buffer.clear();
    unsigned char * tmp_buffer=new unsigned char[size];
    size_t read=fread((char*)tmp_buffer,1, size, file);
    for( std::size_t ix = 0; ix < read; ++ix )
    {
        buffer.push_back(tmp_buffer[ix]);
    }
    _filePointer-=read;
	delete tmp_buffer;
    return read;
}


FileInputStream::~FileInputStream() {
    this->close();
}
