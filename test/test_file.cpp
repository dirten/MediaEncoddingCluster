#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::lang;
int main(int argc, char**argv){

    File * file1=new File("frame1.ppm");
    File * file2=new File("frame.2.ppm");
    File * file3=new File();
    cout << file1->getPath()<<endl;;
    cout << file2->getPath()<<endl;;


    try{
	FileInputStream * is3=new FileInputStream(file3);
	FileInputStream * is1=new FileInputStream(file1);
	FileInputStream * is2=new FileInputStream(file2);
    }catch (Exception &ex){
	ex.printStackTrace();
    }

    cout << file1->getPath()<<endl;;
    cout << file2->getPath()<<endl;;
    delete file1;
    delete file2;
}
