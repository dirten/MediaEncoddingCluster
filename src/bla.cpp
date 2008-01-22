//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp> 
#include <boost/archive/polymorphic_binary_oarchive.hpp> 
#include <fstream>
#include "org/esb/av/Packet.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
//#include "org/esb/io/ObjectOutputStream.h"
#include <iostream>
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace std;
int main(){




{
	int size=10;
        char testdata[10]="bla fasel";
//        sprintf(testdata,"blafasel");
	std::filebuf sb;
	sb.open("bla.txt",ios::out);
        std::ostream os(&sb);
        boost::archive::polymorphic_binary_oarchive ar(os);
	// boost::archive::polymorphic_text_oarchive ar(os);
	// boost::archive::text_oarchive ar(os);
	// boost::archive::binary_oarchive ar(os);
	ar & size;
        ar & boost::serialization::make_binary_object(&testdata,size);
//        assert(os.good());
//        cout << testdata << endl;
//        cout << size << endl;

    
}

{
	int size=10;
//       char * testdata=new char [size];
        char testdata[10]="bla fasel";
//        sprintf(testdata,"blafasel");
        std::ofstream os("test.txt", std::ios::binary);
        boost::archive::polymorphic_binary_oarchive ar(os);
	// boost::archive::polymorphic_text_oarchive ar(os);
	// boost::archive::text_oarchive ar(os);
	// boost::archive::binary_oarchive ar(os);
	ar & size;
        ar & boost::serialization::make_binary_object(&testdata,size);
        assert(os.good());
        cout << testdata << endl;
        cout << size << endl;

}
{
	int size=0;
//        char testdata[100]="";
        std::ifstream is("test.txt", std::ios::binary);
        assert(is.good());
        boost::archive::polymorphic_binary_iarchive ar(is);
	// boost::archive::polymorphic_text_iarchive ar(is);
	// boost::archive::text_iarchive ar(is);
	// boost::archive::binary_iarchive ar(is);
	ar & size;
	char * testdata=new char[size];
        ar & boost::serialization::make_binary_object(testdata,size);	
        assert(is.good());
//        assert(testdata[0] == 'a');
        cout << testdata <<strlen(testdata)<< endl;
        cout << size << endl;
	delete []testdata;
    }
//    return 0; 

//    ifstream ifs("/tmp/hive/data.100",ios::binary);
    ifstream ifs("/tmp/hive/data.100");
//    ifstream ifs("test.ous",ios::binary);
    boost::archive::text_iarchive bis(ifs);
    Packet  packet2;
//    packet2.data=new uint8_t[10000];
    bis >> packet2;
    cout << packet2.getSize() << endl;

    FileOutputStream fos("test.ous2");
    ObjectOutputStream oos(&fos);
    oos.writeObject(packet2);
    oos.close();
    fos.close();
    
    
    
    FileInputStream fis("test.ous2");
    ObjectInputStream ois(&fis);
    Packet packet3;
    ois.readObject(packet3);

    cout << "PacketSize:"<<packet3.getSize()<<endl;

}


