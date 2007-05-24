#include "hive.job.h"
#include <iostream>

using namespace std;
 
char * _inputFilename;
char * _outputFilename;

void HiveJob::setInputFilename(char * filename){
    if(_inputFilename){
	delete _inputFilename;
    }
    _inputFilename=new char [strlen(filename)];
    memcpy(_inputFilename, filename, strlen(filename));
}
	 
const char * HiveJob::getInputFilename(){
    return _inputFilename;
}


void HiveJob::setOutputFilename(char * filename){
    if(_outputFilename){
	delete _outputFilename;
    }
    _outputFilename=new char [strlen(filename)];
    memcpy(_outputFilename, filename, strlen(filename));
}
	 
const char * HiveJob::getOutputFilename(){
    return _outputFilename;
}
