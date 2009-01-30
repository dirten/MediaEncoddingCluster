#include "org/esb/config/config.h"
#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/FormatStreamFactory.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "tntdb/connection.h"
//#include "tntdb/connect.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/archive/xml_oarchive.hpp>
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::sql;
using namespace org::esb::config;


boost::mutex ClientHandler::m_mutex;
boost::mutex ClientHandler::unit_list_mutex;
map<int, boost::shared_ptr<ProcessUnit> > ClientHandler::process_unit_list;

ClientHandler::ClientHandler() {
  logdebug("ClientHandler::ClientHandler()");
  _handler = JobHandler::getInstance();
  _con = new Connection(Config::getProperty("db.connection"));
  _stmt_ps = new PreparedStatement(_con->prepareStatement("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1"));
  _stmt = new PreparedStatement(_con->prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
      "(NULL,:stream_id,:pts,:dts,:stream_index,:key_frame, :frame_group,:flags,:duration,:pos,:data_size,:data)"));
  _stmt_fr = new PreparedStatement(_con->prepareStatement("update process_units set complete = now() where id=:id"));
  _stmt_pu = new PreparedStatement(_con->prepareStatement("update process_units set send = now() where id=:id"));
}

ClientHandler::~ClientHandler() {
  logdebug("ClientHandler::~ClientHandler()");
  delete _con;
  delete _stmt;
  delete _stmt_fr;
  delete _stmt_pu;
  delete _stmt_ps;
}

bool ClientHandler::addProcessUnit(boost::shared_ptr<ProcessUnit> unit) {
  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);
  bool result = false;
  if (process_unit_list.find(unit->id) == process_unit_list.end()) {
    process_unit_list[unit->id] = unit;
    result = true;
  }
  return result;
}

void ClientHandler::fillProcessUnit(ProcessUnit * u) {
  boost::mutex::scoped_lock scoped_lock(m_mutex);
  /*
          logdebug("packing frame group :" <<fr_gr<<" with startts: "<<startts);
          _fis->seek(_stream_index,(int64_t)startts);
          PacketInputStream pis(_fis);
          int size=0;
          for(int a=0;a<frame_count;){
              Packet tmp_p;
              pis.readPacket(tmp_p);
              if(tmp_p.packet->stream_index!=_stream_index)continue;
              a++;
              shared_ptr<Packet> p(new Packet(tmp_p));
              u._input_packets.push_back(p);
              size+=p->packet->size;
          }
          logdebug("packing frame group :" <<fr_gr<<" with size:"<<size<<" !!!");
      }else{
          logdebug("no more frame groups left, setting job as completed");
          setCompleteTime(1);
      }
   */


}

ProcessUnit ClientHandler::getProcessUnit() {
  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);
//  Connection con(Config::getProperty("db.connection"));
  Statement stmt_ps = _con->createStatement("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id limit 1");
  ResultSet rs = stmt_ps.executeQuery();

  ProcessUnit u;
  if (rs.next()) {
    int64_t start_ts = rs.getLong("start_ts");
    int frame_count = rs.getInt("frame_count");
    int stream_index = rs.getInt("stream_index");
    logdebug("packing frame group with startts: " << start_ts);
    string filename = rs.getString("path");
    filename.append("/");
    filename.append(rs.getString("filename"));
    u._decoder = CodecFactory::getStreamDecoder(rs.getInt("source_stream"));
    u._encoder = CodecFactory::getStreamEncoder(rs.getInt("target_stream"));
    u._source_stream = rs.getInt("source_stream");
    u._target_stream = rs.getInt("target_stream");
/*
    org::esb::io::File file(filename.c_str());
    if (!file.exists()) {
      logerror("Could not find file : " << filename.c_str());
      _stmt_pu->setInt("id", rs->getInt("u.id"));
      _stmt_pu->execute();
      return u;
    }
    FormatInputStream fis(&file);
*/
	FormatInputStream * fis=FormatStreamFactory::getInputStream(filename);
        if(fis==NULL){
          logerror("Error Opening Input Stream from "<<filename);
          return u;
        }
//    fis->seek(rs->getInt("stream_index"), (start_ts - 70000));
	int den=rs.getInt("time_base_den");
	int type=rs.getInt("stream_type");
	int framerate=rs.getInt("framerate");
	int samplerate=rs.getInt("sample_rate");
	int gop=rs.getInt("gop_size");
	long offset=0;//(den/(type==CODEC_TYPE_VIDEO?framerate:samplerate))*gop;
	logdebug("building seek offset -"<<offset);
    fis->seek(rs.getInt("stream_index"), (start_ts-offset));
    PacketInputStream pis(fis);
    int size = 0;
    for (int a = 0; a < frame_count + 3;) {
      Packet tmp_p;
      if (pis.readPacket(tmp_p) < 0){
        logdebug("Null Packet from Stream");
        break;
      }
      if (tmp_p.packet->stream_index != stream_index)continue;
      if (tmp_p.packet->dts >= start_ts) {
        a++;
        boost::shared_ptr<Packet> p(new Packet(tmp_p));
        u._input_packets.push_back(p);
        size += p->packet->size;
      }else{
        logdebug("Dropping Packet with dts"<<tmp_p.packet->dts);
      }
    }
    u._process_unit = rs.getInt("u.id");
    _stmt_pu->setInt("id", rs.getInt("u.id"));
    _stmt_pu->execute();
/*
		std::string path="C:/devel/MediaEncodingCluster-build/src/Debug";
		path+="/tmp/";
		std::string outfilename=path.append(org::esb::util::Decimal(rs.getInt("u.id")%1000).toString());
		outfilename+="/";
		org::esb::io::File dir(outfilename.c_str());
		if(!dir.exists()){
			dir.mkdir();
		}

		outfilename.append(rs.getString("u.id")).append(".unit_src");
		std::ofstream ofs(outfilename.c_str());
			boost::archive::binary_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(u);
*/
    logdebug("packing frame group  with size:" << size << " !!!");

  } else {
    //      logdebug("no more process units left, sending empty process unit");
    //        setCompleteTime(1);
  }
  return u;
}

ProcessUnit ClientHandler::getProcessUnit2() {
  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);

  Statement stmt_ps = _con->createStatement("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  ResultSet * rs = stmt_ps.executeQuery2();

  ProcessUnit u;
  if (rs->next()) {
    int64_t start_ts = rs->getLong("start_ts");
    int frame_count = rs->getInt("frame_count");
    int stream_index = rs->getInt("stream_index");
    logdebug("packing frame group with startts: " << start_ts);
    string filename = rs->getString("path");
    filename.append(rs->getString("filename"));
    u._decoder = CodecFactory::getStreamDecoder(rs->getInt("source_stream"));
    u._encoder = CodecFactory::getStreamEncoder(rs->getInt("target_stream"));
    u._source_stream = rs->getInt("source_stream");
    u._target_stream = rs->getInt("target_stream");
    int size = 0;
    Connection con2(Config::getProperty("db.connection"));
    PreparedStatement stmt_p = con2.prepareStatement("select * from packets where stream_id=:sid and dts>=:dts limit :limit");
    stmt_p.setInt("sid", rs->getInt("source_stream"));
    stmt_p.setLong("dts", start_ts);
    stmt_p.setInt("limit", frame_count + 3);
    logdebug("select * from packets where stream_id=" << rs->getInt("source_stream") << " and dts>=" << start_ts << " limit " << frame_count + 3);
    ResultSet rs_p = stmt_p.executeQuery();

    for (int a = 0; rs_p.next();) {
      if (rs_p.getInt("data_size") != rs_p.getBlob("data").length()) {
        logerror("field data_size has different value then the blob field on packet id:" << rs_p.getInt("id") << " : DROP PACKET")
        continue;
      }
      boost::shared_ptr<Packet> p(new Packet(rs_p.getInt("data_size")));
      memcpy(p->packet->data, rs_p.getBlob("data").c_str(), p->packet->size);
      p->packet->pts = rs_p.getLong("pts");
      p->packet->dts = rs_p.getLong("dts");
      p->packet->duration = rs_p.getInt("duration");
      p->packet->flags = rs_p.getInt("flags");
      p->packet->pos = rs_p.getLong("pos");
      p->packet->stream_index = rs_p.getInt("stream_index");
      u._input_packets.push_back(p);
      size += p->packet->size;
    }
    u._process_unit = rs->getInt("u.id");
    _stmt_pu->setInt("id", rs->getInt("u.id"));
    _stmt_pu->execute();
    logdebug("packing frame group  with size:" << size << " !!!");

  } else {
    //      logdebug("no more process units left, sending empty process unit");
    //        setCompleteTime(1);
  }
  return u;
}
/*
string toString(int num) {
  char c[10];
  memset(&c, 0, 10);
  sprintf(c, "%d", num);
  return string(c);
}
*/
bool ClientHandler::putProcessUnit(ProcessUnit & unit) {
  {
    boost::mutex::scoped_lock scoped_lock(m_mutex);
    logdebug("ClientHandler::putProcessUnit(ProcessUnit & unit) : start"<<unit._process_unit);
	/*
	std::string name=org::esb::config::Config::getProperty("hive.path");
	name+="/tmp/";
	name+=org::esb::util::Decimal(unit._process_unit%1000).toString();
	name+="/";
	org::esb::io::File dir(name.c_str());
	if(!dir.exists()){
		dir.mkdir();
	}
	name+=org::esb::util::Decimal(unit._process_unit).toString();
	name+=".unit";
	org::esb::io::File out(name.c_str());
	org::esb::io::FileOutputStream fos(&out);
	org::esb::io::ObjectOutputStream ous(&fos);
//	ous.writeObject(unit);
*/
    _stmt_fr->setInt("id", unit._process_unit);
    _stmt_fr->execute();
	
	list< boost::shared_ptr<Packet> >::iterator it;
    _stmt_fr->setInt("id", unit._process_unit);
    _stmt_fr->execute();
    int count = 0, frame_group = 0;
    for (it = unit._output_packets.begin(); it != unit._output_packets.end(); it++) {
      boost::shared_ptr<Packet> packet = *it;

      if (packet->packet->data == NULL) {
        cout << "Packet data is null???" << endl;
      }
      ++count;
      int field = 0;
      _stmt->setInt("stream_id", unit._target_stream);
      _stmt->setLong("pts", packet->packet->pts);
      _stmt->setLong("dts", packet->packet->dts);
      _stmt->setInt("stream_index", packet->packet->stream_index);
      _stmt->setInt("key_frame", packet->isKeyFrame());
      if (packet->packet->stream_index == 0)
        _stmt->setInt("frame_group", frame_group);
      else
        _stmt->setInt("frame_group", 0);
      _stmt->setInt("flags", packet->packet->flags);
      _stmt->setInt("duration", packet->packet->duration);
      _stmt->setLong("pos", packet->packet->pos);
      _stmt->setInt("data_size", packet->getSize());
      _stmt->setBlob("data", (char *) packet->packet->data, packet->packet->size);
      _stmt->execute();
    }
  }
  logdebug("ClientHandler::putProcessUnit(ProcessUnit & unit) : end");

  return true;
}



