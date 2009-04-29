////////////////////////////////////////////////////////////////////////
// upload.cpp
// generated with ecppc
// date: Tue May 13 14:34:35 2008
//

#include <tnt/ecpp.h>
#include <tnt/convert.h>
#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <tnt/httpheader.h>
#include <tnt/http.h>
#include <tnt/data.h>
#include <tnt/componentfactory.h>
#include <cxxtools/log.h>
#include <stdexcept>

log_define("component.upload")

// <%pre>
#line 1 "upload.ecpp"

#include <fstream>
#include <math.h>
#include <iostream>
#include "import.cpp"
#include "job.cpp"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
using namespace org::esb::sql;
using namespace org::esb::config;

// </%pre>

namespace
{
template <typename T> inline void use(const T&) { }

class _component_upload : public tnt::EcppComponent
{
    _component_upload& main()  { return *this; }

  protected:
    ~_component_upload();

  public:
    _component_upload(const tnt::Compident& ci, const tnt::Urlmapper& um, tnt::Comploader& cl);

    unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
    class fieldstart_type : public tnt::EcppSubComponent
    {
        _component_upload& mainComp;
        _component_upload& main()  { return mainComp; }

      public:
        fieldstart_type(_component_upload& m, const std::string& name)
          : EcppSubComponent(m, name),
            mainComp(m)
          { }
        unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
    };

    class fieldend_type : public tnt::EcppSubComponent
    {
        _component_upload& mainComp;
        _component_upload& main()  { return mainComp; }

      public:
        fieldend_type(_component_upload& m, const std::string& name)
          : EcppSubComponent(m, name),
            mainComp(m)
          { }
        unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
    };

    fieldstart_type fieldstart;
    fieldend_type fieldend;
};

static tnt::ComponentFactoryImpl<_component_upload> uploadFactory("upload");

static const char* rawData = "8\000\000\000\213\000\000\000\242\001\000\000\255\002\000\000\277\002\000\000\301\002\000\000\313\002\000\000\353\002\000\000\354\002\000\000\374\002\000\000\020\003\000\000\032\003\000\000\033\003\000\000)\003\000\000\n<html>\n<head>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style\">\n</head>\n<body>\n<div class=\"menu\"><div class=\"menuleft\"></div>\n<div class=\"menuentry\"><a href=\"upload\">File Upload</a></div>\n<div class=\"menuentry\"><a href=\"file\">File List</a></div>\n<div class=\"menuentry\"><a href=\"profile\">Encoding Profiles</a></div>\n<div class=\"menuright\"></div>\n</div>\n<br>\n\n\n<fieldset>\n<legend>File Upload</legend>\n<form action=\"/upload\" method=\"post\" enctype=\"multipart/form-data\" action=\"/post\">\n    <input type=\"file\" name=\"file\">\n    <input type=\"submit\">\n    <select name=\"profile\">\n    <option value=\"0\">nicht sofort Encoden</option>\n\n\n\t\t<option value=\"\"></option>\n\n\t</select>\n</form>\n</fieldset>\n\n</body>\n<html>\n\n  <fieldset><legend></legend>\n\n  </fieldset>\n";

// <%shared>
// </%shared>

// <%config>
// </%config>

#define SET_LANG(lang) \
     do \
     { \
       request.setLang(lang); \
       reply.setLocale(request.getLocale()); \
     } while (false)

_component_upload::_component_upload(const tnt::Compident& ci, const tnt::Urlmapper& um, tnt::Comploader& cl)
  : EcppComponent(ci, um, cl),
    fieldstart(*this, "fieldstart"),
    fieldend(*this, "fieldend")
{
  // <%init>
  // </%init>
}

_component_upload::~_component_upload()
{
  // <%cleanup>
  // </%cleanup>
}

unsigned _component_upload::operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
{
  tnt::DataChunks data(rawData);

  // <%args>
int profile = qparam.has("profile") ? tnt::stringToWithDefaultConverter<int>(0, reply.out().getloc())(qparam.param("profile")) : 0;
  // </%args>

  // <%cpp>
  reply.out() << data[0]; // \n<html>\n<head>\n<link rel="stylesheet" type="text/css" href="style">\n</head>\n<body>\n
  // <%include> menu.tmpl
  reply.out() << data[1]; // <div class="menu"><div class="menuleft"></div>\n<div class="menuentry"><a href="upload">File Upload</a></div>\n<div class="menuentry"><a href="file">File List</a></div>\n<div class="menuentry"><a href="profile">Encoding Profiles</a></div>\n<div class="menuright"></div>\n</div>\n<br>\n\n
  // </%include>
  // <& fieldstart ...
#line 24 "upload.ecpp"
  tnt::QueryParams _tnt_cq0(qparam, false);
#line 24 "upload.ecpp"
  _tnt_cq0.add( "legend", tnt::toString("test legend", reply.out().getloc() ) );
#line 24 "upload.ecpp"
  main().fieldstart(request, reply, _tnt_cq0);
  // &>
  reply.out() << data[2]; // \n<fieldset>\n<legend>File Upload</legend>\n<form action="/upload" method="post" enctype="multipart/form-data" action="/post">\n    <input type="file" name="file">\n    <input type="submit">\n    <select name="profile">\n    <option value="0">nicht sofort Encoden</option>\n\n
#line 33 "upload.ecpp"

	Connection con(Config::getProperty("db.connection"));
	Statement stmt=con.createStatement("select * from profiles");
    ResultSet rs=stmt.executeQuery();
	while(rs.next()){ 
  reply.out() << data[3]; // \n\t\t<option value="
#line 38 "upload.ecpp"
  reply.sout() << (rs.getInt("id"));
  reply.out() << data[4]; // ">
#line 38 "upload.ecpp"
  reply.sout() << (rs.getString("profile_name"));
  reply.out() << data[5]; // </option>\n
#line 39 "upload.ecpp"
	}  
  reply.out() << data[6]; // \n\t</select>\n</form>\n</fieldset>\n
  // <& fieldend ...
#line 43 "upload.ecpp"
  tnt::QueryParams _tnt_cq1(qparam, false);
#line 43 "upload.ecpp"
  main().fieldend(request, reply, _tnt_cq1);
  // &>
  reply.out() << data[7]; // \n
#line 44 "upload.ecpp"

const tnt::Multipart& mp = request.getMultipart();
tnt::Multipart::const_iterator it = mp.find("file");
if (it != mp.end()){
  	std::cout << "FileSize:"<<it->getSize()<<std::endl;
    std::ofstream out(it->getFilename().c_str());
    for (tnt::Part::const_iterator pi = it->getBodyBegin(); pi != it->getBodyEnd(); ++pi)
      out << *pi;
	out.flush();
    char * argv[]={"",(char*)it->getFilename().c_str()};
    int fileid=import(2,argv);

	if(profile>0){
		char fid[(int)log(fileid)];
		sprintf(fid,"%d",fileid);
		char pid[(int)log(profile)];
		sprintf(pid,"%d",profile);
    	char * jobarg[]={"","",(char*)fid,(char*)pid};
    	jobcreator(3, jobarg);
    }

}

  reply.out() << data[8]; // </body>\n<html>\n\n
  reply.out() << data[11]; // \n
  // <%/cpp>
  return HTTP_OK;
}

unsigned _component_upload::fieldstart_type::operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
{
  tnt::DataChunks data(rawData);
  // <%args>
string legend = tnt::stringToConverter<string>(reply.out().getloc())( qparam.param("legend") );
  // </%args>

  // <%cpp>
  reply.out() << data[9]; //   <fieldset><legend>
#line 74 "upload.ecpp"
  reply.sout() << (legend);
  reply.out() << data[10]; // </legend>\n
  // <%/cpp>
  return HTTP_OK;
}

unsigned _component_upload::fieldend_type::operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
{
  tnt::DataChunks data(rawData);
  // <%args>
  // </%args>

  // <%cpp>
  reply.out() << data[12]; //   </fieldset>\n
  // <%/cpp>
  return HTTP_OK;
}

} // namespace

