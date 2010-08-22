#include <string>

#include <curl/curl.h>

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
  return true;
  LOGDEBUG("Sending CrashReport");
  std::string url = "http://188.40.40.157/submit";

  std::wstring wpath=dump_path;
  std::string path(wpath.begin(),wpath.end());
  path.append("/");
  std::wstring id=minidump_id;
  std::string file(id.begin(), id.end());
  file.append(".dmp");
  //  logdebug("Sending CrashReport"<<path.append(file).c_str());


  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);
  LOGDEBUG("Post file" << path.append(file));
  /* Fill in the file upload field */
  curl_formadd(&formpost,
      &lastptr,
      CURLFORM_COPYNAME, "upload_file_minidump",
      CURLFORM_FILE, path.append(file).c_str(),
      CURLFORM_END);

  /* Fill in the filename field */

  curl_formadd(&formpost,
      &lastptr,
      CURLFORM_COPYNAME, "filename",
      CURLFORM_COPYCONTENTS, file.c_str(),
      CURLFORM_END);


  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append(headerlist, buf);
  if (curl) {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 5);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all(headerlist);
  }
  LOGDEBUG("sended");
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
#pragma message("compile with dumphandler")
&MyDumpSender,
#else
#pragma message("compile without dumphandler")
NULL,
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

