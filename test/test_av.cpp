#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/config/config.h"
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::sql;
using namespace org::esb::config;

using namespace std;
int main(int argc, char ** argv){
	int loop=1;
	while(loop-->0){
	std::cout <<"loop "<<loop<<endl;

//	File f("/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
//	File f("/media/TREKSTOR/videos/20070401 1825 - PREMIERE 3 - Wes Craven prÃ¤sentiert Dracula III_ Legacy (Dracula III_ Legacy).ts");
	org::esb::io::File f("e:/ChocolateFactory.ts");
//	File f("m:\\video\\20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
	FormatInputStream fis(&f);
	fis.seek(0,(3769560833-70000));
	PacketInputStream pis(&fis);
	int a=0;
        bool print=false;
        Packet p;

		Connection con("mysql:host=192.168.0.187;db=hive;user=root;passwd=root");
		PreparedStatement st=con.prepareStatement("select * from packets where dts=:dts");
		while(false||a<50){
		if(pis.readPacket(p)<0)break;
		if(p.packet->stream_index==0){
			if(p.packet->pts>=3769560833){
				st.setLong("dts",p.packet->dts);
				ResultSet rs=st.executeQuery();
				rs.next();
				std::cout <<"PTS:"<< p.packet->pts<<" DTS:"<<p.packet->dts<<" Size "<<rs.getLong("data_size")<<":"<<p.packet->size<<std::endl;
				a++;
			}
		}
		}
	std::cout << endl;
	}
}

