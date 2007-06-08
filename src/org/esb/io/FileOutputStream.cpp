#include "FileOutputStream.h"
#include <iostream>
using namespace org::esb::io;
using namespace std;
FileOutputStream::FileOutputStream(const char * pathname){
    open(pathname, false);
}

FileOutputStream::FileOutputStream(File * file){
    open(file->getPath(), false);
}

FileOutputStream::FileOutputStream(const char * pathname, bool append){
    open(pathname, append);
}

FileOutputStream::FileOutputStream(File * file, bool append){
    open(file->getPath(), append);
}

FileOutputStream::~FileOutputStream(){
	flush();
    close();
}

void FileOutputStream::close(){
    fclose(_file);
}

void FileOutputStream::flush(){
    fflush(_file);
}

void FileOutputStream::write(const char * buffer){
    write((char*)buffer, 1);
}

void FileOutputStream::write(char * buffer, int length){
    fwrite(buffer,1, length, _file);
}

void FileOutputStream::write(const char * buffer, int offset, int length){
    write((char*)buffer+offset, length);
}

void FileOutputStream::open(const char * pathname, bool append){
    const char * mode=append==true?"ab":"wb";
    _file=fopen(pathname, mode);
    if(NULL==_file){
        perror("fileopen");
    }
}
