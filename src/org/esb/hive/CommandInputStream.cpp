#include "CommandInputStream.h"
#include "Command.h"
#include <iostream>
#include <errno.h>
using namespace std;
using namespace org::esb::hive;

CommandInputStream::CommandInputStream(InputStream * is) {
  _source = is;
}

CommandInputStream::~CommandInputStream() {

}

Command & CommandInputStream::readCommand() {
  char buffer;
  string b;
  while (available() > 0) {
    this->read((unsigned char *) & buffer, 1);
    b += buffer;
  }
  //	Command * command=new Command();
  //	char com[b.length()+1];
  //	memset(&com,0,b.length()+1);
  //	memcpy(&com, b.c_str(), b.length());
  _command.setCommand(b);
  return _command;
}

int CommandInputStream::read(vector<unsigned char>&buffer) {
  return _source->read(buffer);
}

int CommandInputStream::read() {
  return _source->read();
}

int CommandInputStream::read(unsigned char * buffer, int length) {
  return _source->read(buffer, length);
}

int CommandInputStream::available(bool isBlocking) {
  return _source->available(isBlocking);
}
