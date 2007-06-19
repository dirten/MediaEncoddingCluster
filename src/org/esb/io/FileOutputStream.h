#ifndef ORG_ESB_IO_FILEOUTPUTSTREAM_H
#define ORG_ESB_IO_FILEOUTPUTSTREAM_H
#include "File.h"
#include "OutputStream.h"
#include <iostream>
namespace org {
    namespace esb {
        namespace io {
class FileOutputStream: public OutputStream {
public:
                FileOutputStream(const char * pathname);
                FileOutputStream(File * file);
                FileOutputStream(const char * pathname, bool append);
                FileOutputStream(File * file, bool append);
                ~FileOutputStream();
                void close();
                void flush();
                void write(const char * buffer);
                void write(char * buffer, int length);
                void write(const char * buffer, int offset, int length);
private:
                FILE * _file;
                void open(const char * pathname, bool append);
            };
        }
    }
}
#endif

