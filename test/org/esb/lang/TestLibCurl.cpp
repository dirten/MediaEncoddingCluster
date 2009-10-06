/* 
 * File:   TestLibCurl.cpp
 * Author: jhoelscher
 *
 * Created on 5. Oktober 2009, 17:14
 */

#include <stdlib.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include "org/esb/util/Log.h"
/*
 * 
 */
int main(int argc, char** argv) {

  CURL *curl;
  CURLcode res;

  
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
  std::string path="./";
  std::string file="53e90197-344d-de8a-33c1fa5f-2ba5aa62.dmp";
  curl_global_init(CURL_GLOBAL_ALL);
  /* Fill in the file upload field */
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "upload_file_minidump",
               CURLFORM_FILE, path.append(file).c_str(),
               CURLFORM_END);



  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, "http://188.40.40.157/submit");

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 5);

curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all (headerlist);
  }
  logdebug("sended");
  return (EXIT_SUCCESS);
}

