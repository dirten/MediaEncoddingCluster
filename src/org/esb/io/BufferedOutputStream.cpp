#include "BufferedOutputStream.h"
#include <string.h>


using namespace org::esb::io;
BufferedOutputStream::BufferedOutputStream(OutputStream * os){
	_buffer_size=8192;
	_count=0;
	_buffer=new char[_buffer_size];
	_os=os;
}

BufferedOutputStream::BufferedOutputStream(OutputStream* os, int bufferSize){
	_buffer_size=bufferSize;
	_count=0;
	_buffer=new char[_buffer_size];
	_os=os;
}

BufferedOutputStream::~BufferedOutputStream(){
	if(_buffer)
		delete[]_buffer;
}

void BufferedOutputStream::write(char * buffer, int length){
	if(length>=_buffer_size){
		flush();
		_os->write(buffer, length);
		return;
	}
	if(length > _buffer_size - _count){
		flush();
	}
	memcpy(_buffer+_count, buffer, length);
	_count+=length;
}

void BufferedOutputStream::write(vector <unsigned char >& buffer){

}

void BufferedOutputStream::write(char buffer){
	
	if(_count>=_buffer_size){
		flush();
	}
	memcpy(_buffer+_count,&buffer,1);
	_count++;
}

void BufferedOutputStream::close(){
	flush();
	_os->flush();
	_os->close();
}

void BufferedOutputStream::flush(){
	if(_count>0){
		_os->write((char*)_buffer, _count);
		_count=0;
	}
	_os->flush();
}
