/* 
 * File:   BaseJSONApiTest.cpp
 * Author: HoelscJ
 *
 * Created on 6. Februar 2012, 14:56
 */

#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/util/Log.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/config/config.h"
#include "org/esb/av/FormatBaseStream.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"

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
  HTTPRequest request(HTTPRequest::HTTP_GET, "/api/v1/file//");
  s.sendRequest(request);

  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
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
  file += "/test.dvd";
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, std::string("/api/v1/media/").append(file));
  s.sendRequest(request);

  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);
  std::string data;
  StreamCopier::copyToString(rs, data);
  JSONNode node = getJson(data);
  assert_response(node);

  assert(node.contains("data"));
  assert(node["data"].size() > 0);
  assert(node["data"].contains("path"));
  assert(node["data"].contains("name"));
  assert(node["data"].contains("size"));
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

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::config::Config::setProperty("HOME",".");
  org::esb::config::Config::setProperty("APPDATA",".");
  org::esb::hive::Environment::build(argc, argv);
  Log::open();
  org::esb::av::FormatBaseStream::initialize();
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(JSONAPI_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  org::esb::core::PluginRegistry::getInstance()->startServerServices();
  test_format_list();
  test_codec_list();
  test_file_list();
  test_mediafile();
  test_profile_create();
  
  test_profile_update();
  test_profile_list();
  test_profile_view();

  test_profile_delete();
  test_profile_delete_fail();
  org::esb::core::PluginRegistry::getInstance()->stopServices();
  org::esb::core::PluginRegistry::close();
  org::esb::config::Config::close();
  return 0;
}

