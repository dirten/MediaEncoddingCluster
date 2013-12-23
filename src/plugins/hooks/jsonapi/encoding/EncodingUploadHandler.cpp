
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/SocketStream.h"

#include "Poco/RegularExpression.h"
#include "Poco/String.h"
#include "../exports.h"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"

#include "org/esb/lang/Thread.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"


#include "org/esb/core/Graph.h"
#include "org/esb/core/GraphParser.h"

#include "org/esb/core/PluginContext.h"
#include "plugins/tasks/streamsource/StreamSource.h"


using org::esb::av::FormatInputStream;
using org::esb::av::FormatOutputStream;
using org::esb::av::PacketInputStream;
using org::esb::av::Packet;
using org::esb::core::Graph;
using org::esb::util::StringTokenizer;

using plugin::StreamSource;


class EncodingUploadPartHandler : public Poco::Net::PartHandler {

public:
  EncodingUploadPartHandler(org::esb::core::Graph & graph):graph(graph){
  }
  void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
    LOGDEBUG("HandlePart");
    if (header.has("Content-Disposition")) {
      LOGDEBUG("Content-Disposition");
      std::string disp;
      Poco::Net::NameValueCollection params;
      Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
      LOGDEBUG("Param-Name" << params.get("name", "(unnamed)"));
      LOGDEBUG("Param-FileName" << params.get("filename", "(unnamed)"));
    }
    if (header.has("Content-Length")) {
      LOGDEBUG("Content-Length");
    }
    LOGDEBUG("Graph elements:"<< graph.getElements().size());

    /**
      * @TODO: remember memleak here, need to be fixed as soon as possible
      * need to be cleaned up after the graph has been destroyed
      * because the graph need to be close the decoder first, after that
      * the InputStream could save destroyed
      */
    fis=new FormatInputStream(stream);

    foreach(Ptr<Graph::Element> el, graph.getElements()) {
      if (el->getParents().size() == 0) {
        LOGDEBUG("Element:"<<el->name);
        StreamSource * source=static_cast<StreamSource*>(el->task.get());
        source->setSource(fis);
      }
    }
    graph.run();
    //delete fis;
    /*
    PacketInputStream pis(&fis);
    org::esb::av::Packet * packet;
    while((packet = pis.readPacket()) != NULL){
      //LOGDEBUG("Packet received");
      delete packet;
    }
    */
  }

  ~EncodingUploadPartHandler(){
    //delete fis;
  }

  std::string getData() {
    return _data;
  }
  uint64_t getLength(){
    return _length;
  }

private:
  std::string _data;
  uint64_t _length;
  org::esb::core::Graph & graph;
  FormatInputStream * fis;
};

class JSONAPI_EXPORT EncodingUploadHandler : public org::esb::core::WebHookPlugin {
org::esb::core::Graph * graphobj;
  db::HiveDb _db;
public:
  EncodingUploadHandler() : _db(db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"))){

  }
  ~EncodingUploadHandler(){
    //pipe.close();
    //delete graphobj;

    LOGDEBUG("~EncodingUploadHandler()");
  }
  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    LOGDEBUG("handle upload");
    JSONResult result(req);
    std::string id = req.get("profileid");
    litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (_db, db::Preset::Uuid == id);
    if (s.count() > 0) {
      LOGDEBUG("preset found");

      db::Preset preset = s.one();
      JSONNode profile=createProfilenode(id,preset.data);
      JSONNode graph(JSON_NODE);
      JSONNode tasks(JSON_ARRAY);
      tasks.set_name("tasks");

      std::string uuid=org::esb::util::PUUID();

      std::string format_id=profile["data"]["format"]["id"].as_string();
      AVOutputFormat * oformat=FormatOutputStream::getOutputFormat(format_id);
      StringTokenizer tok(oformat->extensions,",");
      std::string ext;
      if(tok.hasMoreTokens())
        ext=tok.nextToken();

      std::string outfile="/tmp/"+uuid+"."+ext;
      tasks.push_back(createInfilenode());
      tasks.push_back(createOutfilenode(outfile));
      tasks.push_back(profile);

      graph.push_back(tasks);
      graph.push_back(JSONNode("uuid",id));
      //graph.push_back(JSONNode("name","bla fasel"));
      //LOGDEBUG("default graph"<<graph.write_formatted());
      try{

      org::esb::core::GraphParser graphparser(graph.write_formatted());
      LOGDEBUG("here")
      org::esb::core::GraphParser::ElementMap & el = graphparser.getElementMap();
      std::list<Ptr<org::esb::core::Graph::Element> > list;
      foreach(org::esb::core::GraphParser::ElementMap::value_type & element, el) {
        element.second->task->getContext()->set<std::string>("uuid",uuid);
        list.push_back(element.second);
      }
     org::esb::core::Graph graphobj = org::esb::core::Graph(list, uuid);

      /*create a job entry here*/

      db::Job job(_db);
      job.uuid=uuid;
      job.outfile=outfile;
      job.graph=graph.write_formatted();
      job.update();


       //release this scope before the graph will be freed
        EncodingUploadPartHandler partHandler(graphobj);
        Poco::Net::HTMLForm form(req, req.stream(), partHandler);

      job.status=db::Job::Status::Exporting;
      job.update();
      }catch(std::exception & ex){
        LOGERROR("exception catched:"<<ex.what());
      }

      //delete graphobj;
    }else{
      result.setStatus("failed","Profile not found");
      res.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_NOT_FOUND,"Profile not found");
    }
    res.setChunkedTransferEncoding(true);
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

private:
  JSONNode createInfilenode(){
    JSONNode result;
    result.push_back(JSONNode("name","StreamSource"));
    result.push_back(JSONNode("uid",1));
    /*
    JSONNode data(JSON_NODE);
    data.set_name("data");
    data.push_back(JSONNode("srcurl",filename));
    result.push_back(data);
    */
    JSONNode links(JSON_ARRAY);
    links.set_name("linksTo");
    links.push_back(JSONNode("","3"));
    result.push_back(links);


    return result;
  }

  JSONNode createOutfilenode(std::string filename){
    JSONNode result;
    result.push_back(JSONNode("name","OutputTask"));
    result.push_back(JSONNode("uid",2));

    JSONNode data(JSON_NODE);
    data.set_name("data");
    data.push_back(JSONNode("outfile",filename));
    result.push_back(data);


    return result;
  }
  JSONNode createProfilenode(std::string uuid, std::string jdata){

    JSONNode result;
    result.push_back(JSONNode("name","EncodingTask"));
    result.push_back(JSONNode("uid",3));

    JSONNode data=libjson::parse(jdata);
    data.set_name("data");
    result.push_back(data);

    JSONNode links(JSON_ARRAY);
    links.set_name("linksTo");
    links.push_back(JSONNode("","2"));
    result.push_back(links);

    return result;
  }

  std::string createJob(std::string graphname, std::string graph, std::string infile) {
    db::Job job(_db);
    job.uuid = (std::string)org::esb::util::PUUID();
    job.status = db::Job::Status::Waiting;
    job.graph = graph;
    job.graphname = graphname;
    job.infile = infile;
    job.created = 0;
    job.update();
    return job.uuid;
  }

};
//REGISTER_WEB_HOOK("/api/v1/encoding/?$", PUT, EncodingCreateHandler)
REGISTER_WEB_HOOK("/api/v1/encoding/{profileid}$", POST, EncodingUploadHandler)








