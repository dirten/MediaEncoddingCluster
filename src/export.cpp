#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/File.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::sql;

int main(){
    File fout("/tmp/test.avi");
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);

	Connection con("/tmp/hive.db");
	Statement stmt=con.createStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where stream_id=3 order by pts");
	ResultSet rs=stmt.executeQuery();
	
	
	    CodecID cid=CODEC_ID_MSMPEG4V3;
//    CodecID cid=CODEC_ID_MPEG2VIDEO;

    Encoder *encoder=new Encoder(cid);
    encoder->setBitRate(400000);
    encoder->setTimeBase((AVRational){1,25});
    encoder->setGopSize(10);
    encoder->setPixelFormat(PIX_FMT_YUV420P);
    encoder->setWidth(512);
    encoder->setHeight(256);
    encoder->open();
    pos.setEncoder(*encoder);

	while(rs.next()){
	    Packet p;
	    p.size=rs.getint(0);
	    p.data=new uint8_t[p.size];
	    memcpy(p.data,rs.getblob(1).c_str(),p.size);
	    p.pts=rs.getint(2);
	    p.dts=rs.getint(3);
	    p.duration=rs.getint(4);
	    p.flags=rs.getint(5);
	    p.pos=rs.getint(6);
	    p.stream_index=rs.getint(7);
	    pos.writePacket(p);

	}
return 0;
}