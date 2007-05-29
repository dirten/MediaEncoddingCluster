#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace org::esb::lang;



namespace org {
    namespace esb {
        namespace io {

            /**
            * Creates a new <code>File</code> instance by converting the given
            * pathname string into an abstract pathname.  If the given string is
            * the empty string, then the result is the empty abstract pathname.
            *
            * @param   pathname  A pathname string
            * @throws  NullPointerException
            *          If the <code>pathname</code> argument is <code>null</code>
            */

            File::File(const char * filename) {
                if(filename==NULL) {
                    throw Exception(__FILE__, __LINE__, "File::File - Filename given is NULL");
                }
                _filename=filename;
            }

            File::~File() {}

            const char * File::getPath() {
                return _filename;
            }

            bool File::exist() {
                struct stat attribute;
                return stat(getPath(), &attribute)==0;
            }

            bool File::isFile() {
                struct stat attribute;
//                struct stat attribute=getStat(getPath());
                if(stat(getPath(), &attribute)==0){
                    return S_ISREG(attribute.st_mode);
                }else{
                    return false;
                }
            }

            bool File::isDirectory() {
                struct stat attribute;
                if(stat(getPath(), &attribute)==0){
                    return S_ISDIR(attribute.st_mode);
                }else{
                    return false;
                }
            }

            bool File::canRead() {
//                struct stat attribute=getStat(getPath());
                struct stat attribute;
                if(stat(getPath(), &attribute)==0){

                uid_t uid=getuid();
                gid_t gid=getgid();
                return ((attribute.st_mode&S_IRUSR&&attribute.st_uid==uid)
                        ||(attribute.st_mode&S_IRGRP&&attribute.st_gid==gid)
                        ||attribute.st_mode&S_IROTH);
                }else{
                        return false;
                }
            }

            bool File::canWrite() {
//                struct stat attribute=getStat(getPath());
                struct stat attribute;
                if(stat(getPath(), &attribute)==0){
                uid_t uid=getuid();
                gid_t gid=getgid();
                return ((attribute.st_mode&S_IWUSR&&attribute.st_uid==uid)
                        ||(attribute.st_mode&S_IWGRP&&attribute.st_gid==gid)
                        ||attribute.st_mode&S_IWOTH);
                }else{
                        return false;
                }
            }
        }
    }
}

