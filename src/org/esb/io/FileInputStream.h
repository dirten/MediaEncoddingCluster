#ifndef ORG_ESB_IO_FILEINPUTSTREAM_H
#define ORG_ESB_IO_FILEINPUTSTREAM_H
#include "InputStream.h"
#include "File.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
namespace org {
    namespace esb {
        namespace io {
class FileInputStream : public InputStream {
public:
/**
 * Creates a <code>FileInputStream</code> by
 * opening a connection to an actual file,
 * the file named by the <code>File</code>
 * object <code>file</code> in the file system.
 * A new <code>FileDescriptor</code> object
 * is created to represent this file connection.
 * <p>
 * First, if there is a security manager,
 * its <code>checkRead</code> method  is called
 * with the path represented by the <code>file</code>
 * argument as its argument.
 * <p>
 * If the named file does not exist, is a directory rather than a regular
 * file, or for some other reason cannot be opened for reading then a
 * <code>FileNotFoundException</code> is thrown.
 *
 * @param      file   the file to be opened for reading.
 * @exception  FileNotFoundException  if the file does not exist,
 *                   is a directory rather than a regular file,
 *                   or for some other reason cannot be opened for
 *                   reading.
 * @exception  SecurityException      if a security manager exists and its
 *               <code>checkRead</code> method denies read access to the file.
 * @see        java.io.File#getPath()
 * @see        java.lang.SecurityManager#checkRead(java.lang.String)
 */

                FileInputStream(File * file);
                FileInputStream(const char *);
                ~FileInputStream();
                int available(bool i=false);
                int read(unsigned char * buffer, int length);
                int read(vector<unsigned char>&buffer);
                int read();
                void close();
private:
                void open(const char * name);
                std::FILE * file;
                long _filePointer;
                uint8_t _byte;
            };
        }
    }
}
#endif
