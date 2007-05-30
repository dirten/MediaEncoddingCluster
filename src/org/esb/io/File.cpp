#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;



/**
* Creates a new <code>File</code> instance by converting the given
* pathname string into an abstract pathname.  If the given string is
* the empty string, then the result is the empty abstract pathname.
*
* @param   pathname  A pathname string
* @throws  NullPointerException
*          If the <code>pathname</code> argument is <code>null</code>
*/
File::File(const char * pathname) {
    if(pathname==NULL) {
        throw Exception(__FILE__, __LINE__, "File::File - Filename given is NULL");
    }
    _filename=pathname;
}

File::~File() {}
/**
 * Converts this abstract pathname into a pathname string.  The resulting
 * string uses the {@link #separator default name-separator character} to
 * separate the names in the name sequence.
 *
 * @return  The string form of this abstract pathname
 */

const char * File::getPath() {
    return _filename;
}

/**
 * Tests whether the file or directory denoted by this abstract pathname
 * exists.
 *
 * @return  <code>true</code> if and only if the file or directory denoted
 *          by this abstract pathname exists; <code>false</code> otherwise
 *
 * @throws  SecurityException
 *          If a security manager exists and its <code>{@link
 *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
 *          method denies read access to the file or directory
 */
bool File::exists() {
    struct stat attribute;
    return stat(getPath(), &attribute)==0;
}

/**
 * Tests whether the file denoted by this abstract pathname is a normal
 * file.  A file is <em>normal</em> if it is not a directory and, in
 * addition, satisfies other system-dependent criteria.  Any non-directory
 * file created by a Java application is guaranteed to be a normal file.
 *
 * @return  <code>true</code> if and only if the file denoted by this
 *          abstract pathname exists <em>and</em> is a normal file;
 *          <code>false</code> otherwise
 *
 * @throws  SecurityException
 *          If a security manager exists and its <code>{@link
 *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
 *          method denies read access to the file
 */
bool File::isFile() {
    struct stat attribute;
    //                struct stat attribute=getStat(getPath());
    if(stat(getPath(), &attribute)==0) {
        return S_ISREG(attribute.st_mode);
    } else {
        return false;
    }
}

/**
 * Tests whether the file denoted by this abstract pathname is a
 * directory.
 *
 * @return <code>true</code> if and only if the file denoted by this
 *          abstract pathname exists <em>and</em> is a directory;
 *          <code>false</code> otherwise
 *
 * @throws  SecurityException
 *          If a security manager exists and its <code>{@link
 *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
 *          method denies read access to the file
 */
bool File::isDirectory() {
    struct stat attribute;
    if(stat(getPath(), &attribute)==0) {
        return S_ISDIR(attribute.st_mode);
    } else {
        return false;
    }
}

/**
 * Tests whether the application can read the file denoted by this
 * abstract pathname.
 *
 * @return  <code>true</code> if and only if the file specified by this
 *          abstract pathname exists <em>and</em> can be read by the
 *          application; <code>false</code> otherwise
 *
 * @throws  SecurityException
 *          If a security manager exists and its <code>{@link
 *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
 *          method denies read access to the file
 */
bool File::canRead() {
    //                struct stat attribute=getStat(getPath());
    struct stat attribute;
    if(stat(getPath(), &attribute)==0) {

        uid_t uid=getuid();
        gid_t gid=getgid();
        return ((attribute.st_mode&S_IRUSR&&attribute.st_uid==uid)
                ||(attribute.st_mode&S_IRGRP&&attribute.st_gid==gid)
                ||attribute.st_mode&S_IROTH);
    } else {
        return false;
    }
}

/**
 * Tests whether the application can modify the file denoted by this
 * abstract pathname.
 *
 * @return  <code>true</code> if and only if the file system actually
 *          contains a file denoted by this abstract pathname <em>and</em>
 *          the application is allowed to write to the file;
 *          <code>false</code> otherwise.
 *
 * @throws  SecurityException
 *          If a security manager exists and its <code>{@link
 *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
 *          method denies write access to the file
 */
bool File::canWrite() {
    //                struct stat attribute=getStat(getPath());
    struct stat attribute;
    if(stat(getPath(), &attribute)==0) {
        uid_t uid=getuid();
        gid_t gid=getgid();
        return ((attribute.st_mode&S_IWUSR&&attribute.st_uid==uid)
                ||(attribute.st_mode&S_IWGRP&&attribute.st_gid==gid)
                ||attribute.st_mode&S_IWOTH);
    } else {
        return false;
    }
}

