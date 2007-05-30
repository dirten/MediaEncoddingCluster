#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
#include <exception>
#include <assert.h>
using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::lang;

void my_unexpected(){
    cerr << "Exception is thrown"<<endl;
//    exit(1);
}
void my_terminate(){
    cerr << "Terminator"<<endl;
    exit(1);
}

int main(int argc, char**argv){
    string demo;
    set_unexpected(my_unexpected);
//    set_terminate(my_terminate);
    throw Exception();

    /*
    throw "bla";
//    unexpected();
    try{    
        demo.erase(10,10);
    }catch(Exception &why){
        why.printStackTrace();
//        cerr << "Caught exception " << why.what() << endl;
//        cerr << "We could go on if we wanted" << endl;
//        cerr << "Although in this demo we'll exit nice" << endl;
    }*/
/*    catch(exception &why){
        cerr << "Caught exception " << why.what() << endl;
        cerr << "We could go on if we wanted" << endl;
        cerr << "Although in this demo we'll exit nice" << endl;
    
    }
  */  
    
    File * file1=new File("frame1.ppm");
    cout << file1->getPath()<<endl;;
//    assert(false);
//    File * file2=new File("frame1.ppm");
//    File * file3=new File("frame1.ppm");
//    File * file4=new File("frame1.ppm");
//    File * file2=new File("frame.2.ppm");
//    File * file3=new File();
//    cout << file1->getPath()<<endl;;
//    cout << file2->getPath()<<endl;;
	FormatInputStream * fis1=new FormatInputStream(file1);
	FormatInputStream * fis2=new FormatInputStream(file1);
	FormatInputStream * fis3=new FormatInputStream(file1);
	FormatInputStream * fis4=new FormatInputStream(file1);

	delete fis1;
	delete fis2;
	delete fis3;
	delete fis4;

/*
//    try{
	FileInputStream * is1=new FileInputStream(file1);
//	FileInputStream * is2=new FileInputStream(file1);
	cout <<"size:"<< is1->available() << endl;
	unsigned char tmp[10];
//	memset(tmp,1,10);
	is1->read(tmp, 10);
	cout <<tmp<<endl;
	cout <<"size:"<< is1->available() << endl;
//	delete tmp;
	delete is1;
//	delete is2;
//	delete fis1;
//	FileInputStream * is2=new FileInputStream(file2);
//	FileInputStream * is3=new FileInputStream(file3);
//    }catch (Exception &ex){
//	ex.printStackTrace();
//    }
*/
//    cout << file1->getPath()<<endl;;
//    cout << file2->getPath()<<endl;;
    delete file1;
//    delete file2;
//    delete file3;
//    delete file4;
//    delete file2;
}
