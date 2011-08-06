#ifndef ORG_ESB_IO_FILEOUTPUTSTREAM_H
#define ORG_ESB_IO_FILEOUTPUTSTREAM_H
#include "File.h"
#include "OutputStream.h"
#include <iostream>
#include <vector>
#include "org/esb/io/exports.h"
namespace org {
    namespace esb {
        namespace io {
class IO_EXPORT FileOutputStream: public OutputStream {
public:
                FileOutputStream(std::string pathname);
                FileOutputStream(File * file);
                FileOutputStream(const char * pathname, bool append);
                FileOutputStream(File * file, bool append);
                ~FileOutputStream();
                void close();
                void flush();
                void write(const char * buffer);
                void write(const std::string&);
                void write(char * buffer, int length);
                void write(const char * buffer, int offset, int length);
                void write(vector<unsigned char>&buffer);
private:
                FILE * _file;
                void open(const std::string pathname, bool append);
            };
        }
    }
}
#endif

