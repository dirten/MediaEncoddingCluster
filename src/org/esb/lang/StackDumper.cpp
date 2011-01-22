#include <string>

//#include <curl/curl.h>

#include "StackDumper.h"
#ifndef __WIN32__
#ifdef __APPLE__
#include "client/mac/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#include "common/linux/google_crashdump_uploader.h"
#endif
#elif defined __WIN32__
#include "client/windows/handler/exception_handler.h"
#endif

#include "org/esb/util/Log.h"

#ifdef __WIN32__

bool MyDumpSender(const wchar_t* dump_path,
    const wchar_t* minidump_id,
    void* context,
    EXCEPTION_POINTERS* exinfo,
    MDRawAssertionInfo* assertion,
    bool succeeded) {
#else
bool MyDumpSender(const char *dump_path,
    const char *minidump_id,
    void *context,
    bool succeeded) {
#endif
  
//  LOGDEBUG("Sending CrashReport");
  std::string url = "http://codergrid.de/dump.php";
#ifdef __WIN32__
  std::wstring wpath=dump_path;
  std::string path(wpath.begin(),wpath.end());
  path.append("/");
  std::wstring id=minidump_id;
  std::string file(id.begin(), id.end());
  file.append(".dmp");
#else
  
  std::string path=dump_path;
  path.append("/");
  std::string file=minidump_id;
  file.append(".dmp");
#endif
  //  logdebug("Sending CrashReport"<<path.append(file).c_str());

  for(int a=0;a<10000;a++);
/*
  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  //struct curl_slist *headerlist = NULL;
  //static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);
  path.append(file);
  LOGDEBUG("Post file from Path" << path);

  curl_formadd(&formpost,
      &lastptr,
      CURLFORM_COPYNAME, "upload_file_minidump",
      CURLFORM_FILE, path.c_str(),
      CURLFORM_END);


  curl_formadd(&formpost,
      &lastptr,
      CURLFORM_COPYNAME, "filename",
      CURLFORM_COPYCONTENTS, file.c_str(),
      CURLFORM_END);


  curl = curl_easy_init();
  //headerlist = curl_slist_append(headerlist, buf);
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    std::cout<<"Data present, try to send dump"<<std::endl;
    res = curl_easy_perform(curl);
   std::cout<<"dump sended, result="<<res<<std::endl;;

    curl_easy_cleanup(curl);
    std::cout << "curl cleaned up"<<std::endl;
//    curl_formfree(formpost);
    std::cout << "form cleaned up"<<std::endl;
    //curl_slist_free_all(headerlist);

  }*/
  std::cout<<"sended"<<std::endl;

  return true;
}

StackDumper::StackDumper(std::string dmp_path) 
 {
exhandler=boost::shared_ptr<google_breakpad::ExceptionHandler>(new google_breakpad::ExceptionHandler(
#ifndef __WIN32__
dmp_path,
#elif defined __WIN32__
std::wstring(dmp_path.begin(), dmp_path.end()),
#endif
NULL,
#if defined(__LINUX__) || defined(__WIN32__)
&MyDumpSender,
#else
&MyDumpSender,
#endif
NULL,
#ifndef __WIN32__
true
#elif defined __WIN32__
google_breakpad::ExceptionHandler::HANDLER_ALL
#endif
#if defined(__APPLE__)
,NULL
#endif
));
}

StackDumper::~StackDumper() {

}

