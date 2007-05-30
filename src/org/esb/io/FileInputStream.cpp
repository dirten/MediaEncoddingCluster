#include "FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>


using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
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
FileInputStream::FileInputStream(File * file)throw (Exception) {
    const char * name=file!=NULL?file->getPath():NULL;
    if(!name) {
        throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
    }
    open(name);
}



/**
 * Opens the specified file for reading.
 * @param name the name of the file
 */

void FileInputStream::open(const char * name) {
    file=fopen(name,"r+b");
    if(!file) {
        string error="FileInputStream::open - File not Found (";
        error+=name;
        error+=")";
        throw Exception(__FILE__, __LINE__, error.c_str());
    }
}

void FileInputStream::close() {
    fclose(file);
}

int FileInputStream::available(bool isBlocking) {
    long int currentPosition=ftell(file);
    fseek (file, 0, SEEK_END);
    long int filesize= ftell(file);
    fseek (file, 0, currentPosition);
    return filesize-currentPosition;
}

int FileInputStream::read(unsigned char * buffer, int length) {
    fgets((char*)buffer, length, file);
    return length;
}


FileInputStream::~FileInputStream() {
    this->close();
}
