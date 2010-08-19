#include <string>

#include "StackDumper.h"
#include "org/esb/util/Log.h"
#ifndef __WIN32__

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
//#define OWNCURL
#endif

#ifdef __WIN32__
/*
bool MyDumpSender(const wchar_t* dump_path,
    const wchar_t* minidump_id,
    void* context,
    EXCEPTION_POINTERS* exinfo,
    MDRawAssertionInfo* assertion,
    bool succeeded) {
  return true;
  LOGDEBUG("Sending CrashReport");
  std::string chkpfile = "checkpoint.txt";

  std::string url = "http://188.40.40.157/submit";
  std::wstring dumpfile = dump_path;
  std::string t = "/";
  std::string d = ".";
  std::string dmp = "dmp";
  std::string p = "ProductName";
  std::string v = "Version";
  std::string b = "BuildID";
  std::string product = "MediaEncodingCluster";
  std::string version = "0.0.4.5";
  std::string buildid = __DATE__ "-" __TIME__;

  std::wstring wpro(product.begin(), product.end());
  std::wstring wver(version.begin(), version.end());
  std::wstring wbid(buildid.begin(), buildid.end());
  std::wstring wp(p.begin(), p.end());
  std::wstring wv(v.begin(), v.end());
  std::wstring wb(b.begin(), b.end());

  dumpfile.append(std::wstring(t.begin(), t.end()));
  dumpfile.append(minidump_id);
  dumpfile.append(std::wstring(d.begin(), d.end()));
  dumpfile.append(std::wstring(dmp.begin(), dmp.end()));
  LOGDEBUG(std::string(dumpfile.begin(), dumpfile.end()));
  std::map<std::wstring, std::wstring> para;
  para[wp] = wpro;
  para[wv] = wver;
  para[wb] = wbid;
  std::wstring wresult;
  google_breakpad::CrashReportSender sender(std::wstring(chkpfile.begin(), chkpfile.end()));
  google_breakpad::ReportResult r = sender.SendCrashReport(std::wstring(url.begin(), url.end()), para, dumpfile, &wresult);
  std::string result(wresult.begin(), wresult.end());
  LOGDEBUG("CrashReport sended : " << result << ":::" << r);
  return true;
}*/
#elif defined __LINUX__

bool MyDumpSender(const char *dump_path,
    const char *minidump_id,
    void *context,
    bool succeeded) {
  return true;
  LOGDEBUG("Sending CrashReport");
  std::string url = "http://188.40.40.157/submit";

  std::string path = dump_path;
  path.append("/");
  std::string file = minidump_id;
  file.append(".dmp");
  //  logdebug("Sending CrashReport"<<path.append(file).c_str());

#ifndef OWNCURL
  std::string buildid = __DATE__ "-" __TIME__;
  google_breakpad::GoogleCrashdumpUploader sender("MediaEncodingCluster", "0.0.4.5", __DATE__ "-" __TIME__, "", "", "email", "comments", path.append(file).c_str(), url.c_str(), "", "");
  sender.Upload();


#else

  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);
  logdebug("Post file" << path.append(file));
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
  logdebug("sended");
#endif
}
#endif

StackDumper::StackDumper(std::string dmp_path) :
exhandler(
#ifndef __WIN32__
dmp_path,
#elif defined __WIN32__
std::wstring(dmp_path.begin(), dmp_path.end()),
#endif
NULL,
#if defined(__LINUX__) || defined(__WIN32__)
NULL,
#else
NULL,
#endif
NULL,
#ifndef __WIN32__
true
#elif defined __WIN32__
google_breakpad::ExceptionHandler::HANDLER_ALL
#endif
#if defined(__APPLE__)
NULL;
#endif
) {
  //  exhandler->WriteMinidump(".",&MyDumpSender,NULL);
}
#ifdef WIN32

bool StackDumper::DumpSender(const wchar_t* dump_path,
    const wchar_t* minidump_id,
    void* context,
    EXCEPTION_POINTERS* exinfo,
    MDRawAssertionInfo* assertion,
    bool succeeded) {
  LOGDEBUG("Sending CrashReport");
  return true;
}
#endif

StackDumper::~StackDumper() {

}

