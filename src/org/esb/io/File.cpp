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



File::File(const char * pathname) {
    if(pathname==NULL) {
        throw Exception(__FILE__, __LINE__, "File::File - Filename given is NULL");
    }
    _filename=pathname;
}

File::~File() {}

const char * File::getPath() {
    return _filename;
}

bool File::exists() {
    struct stat attribute;
    return stat(getPath(), &attribute)==0;
}

bool File::isFile() {
    struct stat attribute;
    //                struct stat attribute=getStat(getPath());
    if(stat(getPath(), &attribute)==0) {
        return S_ISREG(attribute.st_mode);
    } else {
        return false;
    }
}

bool File::isDirectory() {
    struct stat attribute;
    if(stat(getPath(), &attribute)==0) {
        return S_ISDIR(attribute.st_mode);
    } else {
        return false;
    }
}

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

