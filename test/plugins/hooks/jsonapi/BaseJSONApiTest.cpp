/*
 * File:   BaseJSONApiTest.cpp
 * Author: HoelscJ
 *
 * Created on 6. Februar 2012, 14:56
 */


#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/util/Log.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/config/config.h"
#include "org/esb/av/FormatBaseStream.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/StreamCopier.h"
#include "Poco/CountingStream.h"
#include "Poco/Process.h"

#include "org/esb/libjson/libjson.h"
using namespace std;
using namespace Poco;
using namespace Poco::Net;
#define HOST "localhost"
#define PORT 4000

JSONNode getJson(std::string data) {
  return libjson::parse(data);
}

void assert_response(JSONNode & response) {
  assert(response.contains("response"));
  assert(response["response"].contains("status"));
  assert(response["response"].contains("message"));
  assert(response["response"].contains("requestID"));
}
#include "JSONProfileTests.cpp"

void test_codec_list() {
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, "/api/v1/codec");
  s.sendRequest(request);
  
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
  JSONNode node = getJson(data);
  assert_response(node);

  assert(node.contains("data"));
  assert(node["data"].size() > 0);
  assert(node["data"].at(0).contains("longname"));
  assert(node["data"].at(0).contains("id"));
  assert(node["data"].at(0).contains("type"));
}

void test_format_list() {
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, "/api/v1/format");
  s.sendRequest(request);

  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
  JSONNode node = getJson(data);
  assert_response(node);

  assert(node.contains("data"));
  assert(node["data"].size() > 0);
  assert(node["data"].at(0).contains("longname"));
  assert(node["data"].at(0).contains("id"));
}

void test_file_list() {
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, "/api/v1/file/");
  s.sendRequest(request);

  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
    LOGDEBUG("response:"<<data);

  JSONNode node = getJson(data);
  
  assert_response(node);

  assert(node.contains("data"));
  assert(node["data"].size() > 0);
  assert(node["data"].at(0).contains("path"));
  assert(node["data"].at(0).contains("name"));
  assert(node["data"].at(0).contains("directory"));
}

void test_mediafile() {
  std::string file = MEC_SOURCE_DIR;
  file += "/test-data/test.dvd";
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, std::string("/api/v1/media/").append(file));

  s.sendRequest(request);

  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);

  JSONNode node = getJson(data);
  assert_response(node);

  assert(node.contains("data"));
  assert(node["data"].size() > 0);
  assert(node["data"].contains("path"));
  assert(node["data"].contains("name"));
  //assert(node["data"].contains("size"));
  assert(node["data"].contains("format"));
  assert(node["data"].contains("duration"));
  assert(node["data"].contains("bitrate"));
  assert(node["data"].contains("streams"));
  LOGDEBUG("StreamCount:" << node["data"]["streams"].size());
  assert(node["data"]["streams"].size() == 2);
  assert(node["data"]["streams"].at(0).contains("codecid"));
  assert(node["data"]["streams"].at(0).contains("type"));
  assert(node["data"]["streams"].at(0).contains("stream_index"));
  assert(node["data"]["streams"].at(0).contains("bitrate"));
  assert(node["data"]["streams"].at(0).contains("timebase"));
  assert(node["data"]["streams"].at(0).contains("framerate"));

  assert(node["data"]["streams"].at(1).contains("codecid"));
  assert(node["data"]["streams"].at(1).contains("type"));
  assert(node["data"]["streams"].at(1).contains("stream_index"));
  assert(node["data"]["streams"].at(1).contains("bitrate"));
  assert(node["data"]["streams"].at(1).contains("samplerate"));
  assert(node["data"]["streams"].at(1).contains("channels"));

}
void test_upload_encoding(){
  std::string file = MEC_SOURCE_DIR;
  file += "/test-data/test.dvd";
  file="/media/video/ChocolateFactory.ts";
  std::vector<std::string> args;
  args.push_back("-F file=@"+file+"");
  args.push_back(std::string("http://localhost:4000/api/v1/encoding/").append(created_profile_uuid));
  Poco::ProcessHandle handle=Poco::Process::launch("curl", args);
  /*
  Poco::ProcessHandle handle2=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle3=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle4=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle5=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle6=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle7=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle8=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle9=Poco::Process::launch("curl", args);
  Poco::ProcessHandle handle0=Poco::Process::launch("curl", args);
*/
  handle.wait();
  /*
  handle2.wait();
  handle3.wait();
  handle4.wait();
  handle5.wait();
  handle6.wait();
  handle7.wait();
  handle8.wait();
  handle9.wait();
  handle0.wait();
  */
  /*
  handle=Poco::Process::launch("curl", args);
  handle.wait();
  handle=Poco::Process::launch("curl", args);
  handle.wait();
  handle=Poco::Process::launch("curl", args);
  handle.wait();
  */
  //Thread::sleep(2000);
  
  /*
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_POST, std::string("/api/v1/encoding/").append(created_profile_uuid));

  HTMLForm form(HTMLForm::ENCODING_MULTIPART);

  FilePartSource * source= new FilePartSource(file);
  form.addPart("file", source);
  form.add("filename","bla");

  form.prepareSubmit(request);
  //CountingOutputStream _cos(s.sendRequest(request));

  std::ostream& send = s.sendRequest(request);
  form.write(send);

  //LOGDEBUG("data written:"<<_cos.chars())
    
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
      */
}

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::config::Config::setProperty("HOME",".");
  org::esb::config::Config::setProperty("APPDATA",".");
  org::esb::hive::Environment::build(argc, argv);
  org::esb::hive::Environment::set("webport","4000");
  Log::open();
  org::esb::av::FormatBaseStream::initialize();
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(JSONAPI_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(STREAMTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(OUTPUTTASK_PLUGIN);

  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  org::esb::core::PluginRegistry::getInstance()->startServerServices();
  /*
  test_format_list();
  test_codec_list();
  test_file_list();
  test_mediafile();
  */
  test_profile_create();
  
  test_profile_update();
  //test_profile_list();
  //test_profile_view();

  //test_profile_delete();
  //test_profile_delete_fail();
  test_upload_encoding();
  return 0;
  org::esb::core::PluginRegistry::getInstance()->stopServices();
  org::esb::core::PluginRegistry::close();
  org::esb::config::Config::close();
  return 0;
}

