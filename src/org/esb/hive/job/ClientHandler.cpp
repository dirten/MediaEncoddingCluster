#include "org/esb/config/config.h"
#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/av/FormatStreamFactory.h"
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
//#include <boost/serialization/is_abstract.hpp>
#include <boost/archive/xml_oarchive.hpp>
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::sql;
using namespace org::esb::config;
using namespace org::esb;

boost::mutex ClientHandler::m_mutex;
boost::mutex ClientHandler::unit_list_mutex;
map<int, boost::shared_ptr<ProcessUnit> > ClientHandler::process_unit_list;
util::Queue<boost::shared_ptr<ProcessUnit> > ClientHandler::puQueue;

ClientHandler::ClientHandler() {
  //	mysql_thread_init();
  logdebug("ClientHandler::ClientHandler()");
  //  _handler = JobHandler::getInstance();
  std::string c = Config::getProperty("db.connection");
  _con = new Connection(c);
  //  _con3 = new Connection(c);
  //  _con4 = new Connection(c);
  //  _con2 = new Connection(c);
  //  _stmt_ps = _con->prepareStatement2("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  //  _stmt3 = _con2->prepareStatement2("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id /*order by priority*/ limit 1");
  _stmt = _con->prepareStatement2("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
      "(NULL,:stream_id,:pts,:dts,:stream_index,:key_frame, :frame_group,:flags,:duration,:pos,:data_size,:data)");
  _stmt_fr = _con->prepareStatement2("update process_units set complete = now() where id=:id");
  _stmt_pu = _con->prepareStatement2("update process_units set send = now() where id=:id");
  _stmt_p = _con->prepareStatement2("select * from packets where stream_id=:sid and dts>=:dts limit :limit");
  //  _stmt_job_log = new PreparedStatement(_con->prepareStatement("insert into process_units(source_stream, target_stream, start_ts,frame_count,send) values (:source, :target, :start, :fcount, now())"));
}

ClientHandler::~ClientHandler() {
  logdebug("ClientHandler::~ClientHandler()");

  delete _con;
  delete _stmt;
  delete _stmt_p;
  delete _stmt_fr;
  delete _stmt_pu;
}

bool ClientHandler::addProcessUnit(boost::shared_ptr<ProcessUnit> unit) {
  //  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);
  puQueue.enqueue(unit);
  return true;
}

void ClientHandler::fillProcessUnit() {

//  boost::mutex::scoped_lock scoped_lock(m_mutex);

}

bool ClientHandler::getProcessUnit(ProcessUnit & u) {
  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);

  //  Statement stmt_ps = _con->createStatement("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  //  ResultSet * rs = stmt_ps.executeQuery2();
  Connection con(Config::getProperty("db.connection"));
  //  PreparedStatement *s = con.prepareStatement2("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  PreparedStatement *s = con.prepareStatement2("select u.id,start_ts, frame_count, stream_index, path,filename, source_stream, target_stream,time_base_num,time_base_den from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  //  ResultSet * rs = stmt_ps->executeQuery("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  ResultSet * rs_pu = s->executeQuery2();
  if (rs_pu->next()) {
    int64_t start_ts = rs_pu->getLong("start_ts");
    int frame_count = rs_pu->getInt("frame_count");
    int stream_index = rs_pu->getInt("stream_index");
    string filename = rs_pu->getString("path");
    filename.append(rs_pu->getString("filename"));
    u._decoder = CodecFactory::getStreamDecoder(rs_pu->getInt("source_stream"));
    u._encoder = CodecFactory::getStreamEncoder(rs_pu->getInt("target_stream"));
    u._source_stream = rs_pu->getInt("source_stream");
    u._target_stream = rs_pu->getInt("target_stream");
    int size = 0;
    //    Connection con2(Config::getProperty("db.connection"));
    //    PreparedStatement * stmt_p = _con3->prepareStatement2("select * from packets where stream_id=:sid and dts>=:dts limit :limit");
    _stmt_p->setInt("sid", rs_pu->getInt("source_stream"));
    _stmt_p->setLong("dts", start_ts);
    _stmt_p->setInt("limit", frame_count + 3);
    logdebug("select * from packets where stream_id=" << rs_pu->getInt("source_stream") << " and dts>=" << start_ts << " limit " << frame_count + 3);
    ResultSet rs_p = _stmt_p->executeQuery();

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
      AVRational ar;
      ar.num = rs_pu->getInt("time_base_num");
      ar.den = rs_pu->getInt("time_base_den");
      p->setTimeBase(ar);
      u._input_packets.push_back(p);
      size += p->packet->size;
    }
    u._process_unit = rs_pu->getInt("u.id");
    _stmt_pu->setInt("id", rs_pu->getInt("u.id"));
    _stmt_pu->execute();
    logdebug("packing frame group  with size:" << size << " !!!");


  } else {
    //      logdebug("no more process units left, sending empty process unit");
    //        setCompleteTime(1);
  }
  delete s;
  //  delete rs_pu;
  //  delete con234;
  return true;

}

ProcessUnit ClientHandler::getProcessUnit() {
  boost::mutex::scoped_lock scoped_lock(unit_list_mutex);

  //  Statement stmt_ps = _con->createStatement("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");
  //  ResultSet * rs = stmt_ps.executeQuery2();
  Connection * con = new Connection(Config::getProperty("db.connection"));
  Statement * stmt_ps = con->createStatement();
  ResultSet * rs = stmt_ps->executeQuery("select * from process_units u, streams s, files f where u.send is null and u.source_stream=s.id and s.fileid=f.id order by priority limit 1");

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
      AVRational ar;
      ar.num = rs->getInt("time_base_num");
      ar.den = rs->getInt("time_base_den");
      p->setTimeBase(ar);
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
    logdebug(__FUNCTION__<<":ClientHandler::putProcessUnit(ProcessUnit & unit) : start" << unit._process_unit);
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



