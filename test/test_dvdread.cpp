//#include <dvdread/dvd_reader.h>
#include <iostream>
#include "org/esb/config/config.h"

using namespace std;
using namespace org::esb::config;
size_t getMegaBytes(size_t);
int main(int argc, char**argv){
    int blocks_to_read=1;
    Config::init("/home/jhoelscher/bruteripper-0.2/config.rc");
    char * sourcedir=Config::getProperty("sourcedir");
    cout << "opening sourcedir:"<< sourcedir << endl;
/*    dvd_reader_t*dvd= DVDOpen(sourcedir);
    DVDInit();    
    dvd_read_domain_t type;
    dvd_file_t * dvd_file=DVDOpenFile( dvd, 1, type );

    size_t size=DVDFileSize(dvd_file);
    cout << "dvd file size:"<< getMegaBytes(size) << endl;

    unsigned char * data=new unsigned char [2048*blocks_to_read];

    size_t t=DVDReadBlocks(dvd_file,0,blocks_to_read,data);
    cout <<"read blocks"<<getMegaBytes(t)<<endl;
    cout << t;
*/
}
size_t getMegaBytes(size_t val){
    return (val/1024)/1024;
}


