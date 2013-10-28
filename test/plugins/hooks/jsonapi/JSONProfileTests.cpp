

std::string created_profile_uuid;
void test_profile_create(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_POST, "/api/v1/profile/");
  //std::ostringstream os;

  
  JSONNode base(JSON_NODE);
  base.push_back(JSONNode("name","test profile"));
  base.push_back(JSONNode("description","test description"));

  JSONNode format(JSON_NODE);
  format.set_name("format");
  format.push_back(JSONNode("id","mp4"));
  base.push_back(format);

  
  JSONNode video(JSON_NODE);
  video.set_name("video");
  video.push_back(JSONNode("id","mpeg4"));

  base.push_back(video);
  
  JSONNode audio(JSON_NODE);
  audio.set_name("audio");
  audio.push_back(JSONNode("id","mp2"));
  audio.push_back(JSONNode("ar","44100"));
  audio.push_back(JSONNode("ac","2"));

  base.push_back(audio);

  //request.write(os);
  std::ostream & os=s.sendRequest(request);
  os << base.write_formatted();
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);


  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);
  assert(node.contains("uuid"));
  created_profile_uuid=node["uuid"].as_string();
}

void test_profile_update(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_PUT, std::string("/api/v1/profile/").append(created_profile_uuid));
  JSONNode base(JSON_NODE);
  base.push_back(JSONNode("name","test profile"));
  base.push_back(JSONNode("description","test description"));

  JSONNode format(JSON_NODE);
  format.set_name("format");
  format.push_back(JSONNode("id","matroska"));
  base.push_back(format);

  
  JSONNode video(JSON_NODE);
  video.set_name("video");
  video.push_back(JSONNode("id","mpeg4"));

  base.push_back(video);
  
  JSONNode audio(JSON_NODE);
  audio.set_name("audio");
  audio.push_back(JSONNode("id","mp2"));
  audio.push_back(JSONNode("ar","44100"));
  audio.push_back(JSONNode("ac","2"));

  base.push_back(audio);

  //request.write(os);
  std::ostream & os=s.sendRequest(request);
  os << base.write_formatted();
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);


  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);
  assert(node.contains("uuid"));
  assert(node["uuid"]==created_profile_uuid);
}

void test_profile_delete(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_DELETE, std::string("/api/v1/profile/").append(created_profile_uuid));
  s.sendRequest(request);
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);


  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);  
  assert(node["response"]["status"]=="ok");
}

void test_profile_delete_fail(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_DELETE, std::string("/api/v1/profile/").append(created_profile_uuid));
  s.sendRequest(request);
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);


  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);  
  assert(node["response"]["status"]=="error");
  
}

void test_profile_list(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, std::string("/api/v1/profile/"));
  s.sendRequest(request);
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);

  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);  
  
}

void test_profile_view(){
  HTTPClientSession s(HOST, PORT);
  HTTPRequest request(HTTPRequest::HTTP_GET, std::string("/api/v1/profile/").append(created_profile_uuid));
  s.sendRequest(request);
  HTTPResponse response;
  std::istream& rs = s.receiveResponse(response);

  std::string data;
  StreamCopier::copyToString(rs, data);
  LOGDEBUG("response:"<<data);
  JSONNode node = getJson(data);
  assert_response(node);  

  assert(node.contains("data"));
  assert(node["data"].contains("format"));
  assert(node["data"]["format"].contains("id"));
  assert(node["data"]["format"]["id"]=="matroska");

  assert(node["data"].contains("video"));
  assert(node["data"]["video"].contains("id"));
  assert(node["data"]["video"]["id"]=="mpeg4");

  assert(node["data"].contains("audio"));
  
}
