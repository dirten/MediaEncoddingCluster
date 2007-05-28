#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <wchar.h>
using namespace std;
int main(int argc,char**argv){
    /*
	    fstream FileBin("/tmp/frame.container/1/frame1.fstream.ppm",ios::in|ios::out|ios::binary);
	    FileBin.seekg(0,ios::end);
	    unsigned long filesize=streamoff(FileBin.tellg());
	    FileBin.seekg(0,ios::beg);
	    string strBuffer="";
	    char buffer[filesize];
	    wchar_t buffer2[filesize];
	    FileBin.read(buffer, filesize);
	    sscanf(buffer,"%s",buffer2);
	    
//	    cout <<"Buffer:"<< buffer << endl;
	    printf("FileSize:%d->%d\n", wcslen(buffer2), filesize);
	    int i=0;
	    while(!(strcmp(buffer+i,"\0")==0)){
//		(buffer+i)="\\0";
		i++;
	    }
	    cout << i<< endl;

*/



//	    FILE * f=fopen("/tmp/frame.container/1/frame1.data.ppm","r+b");
	    int f=open("/tmp/frame.container/1/frame100.fstream.ppm",O_RDONLY);
	    if(!(f)){
		cout << "Datei nicht geöffnet"<<endl;
		return 0;
	    }
	    unsigned char *tmpbuffer=new unsigned char[4000000];
//	    memset(tmpbuffer,0,4000000);
	    int *buffer=new int[1024];
//	    unsigned char c;
//	    for(int i=0;c=fgetc(f);i++){
//	    for(int i=0;fread(buffer,1,255,f)!=0;i++){
	    int byteCount=0;
	    int count=0;
//	    while(count=read(f,buffer,255)){
	    string bla;
 	    for(int i=0;(count=read(f,buffer,1024));i++){
//		cout << count<<endl;;
//		bla.insert(byteCount, buffer);
		memcpy(tmpbuffer+byteCount,buffer,count);
		byteCount+=count;
//		fread(buffer,1,1,f);
//		bla.insert(byteCount, buffer);
		bla+=(char*)buffer;
//		tmpbuffer+=count;
//		cout<<buffer;
//		tmpbuffer[i]=c;
	    }
	    
	    cout<<"ByteCount"<<byteCount<<endl;
	    cout<<"Plan:"<<bla.size()<<endl;
//	    cout << "Buffer:"<<bla<<endl;
	    int filesize=byteCount;
//	    buffer=bla.c_str();

    return 0;
}
    
