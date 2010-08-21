#include "StackDumpUploader.h"
#include "org/esb/util/Log.h"
// Copyright (c) 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <assert.h>
#include <stdarg.h>
#include <wchar.h>

#include <string>

// Disable exception handler warnings.
#pragma warning( disable : 4530 )

#include <fstream>

// See comment in symupload.cc about #undef ERROR. Unfortunately this has to 
// violate style guide and come after http_upload.h.
#undef ERROR 

namespace org {
  namespace esb{
    namespace lang{

using std::ifstream;
using std::ios;

static const wchar_t kUserAgent[] = L"Breakpad/1.0 (Windows)";

wchar_t lastErrorMessageBuffer[1024];

// Helper method to convert Last Error into a text message.  Uses a static
// buffer, so don't save the message across Win32 calls that might change
// the last error.
//
// This function saves/restores the last error and it isn't thread safe.

// Helper class which closes an internet handle when it goes away
class StackDumpUploader::AutoInternetHandle {
 public:
  explicit AutoInternetHandle(HINTERNET handle) : handle_(handle) {}
  ~AutoInternetHandle() {
    if (handle_) {
      InternetCloseHandle(handle_);
    }
  }

  HINTERNET get() { return handle_; }

 private:
  HINTERNET handle_;
};

// static
bool StackDumpUploader::SendRequest(const wstring &url,
                             const map<wstring, wstring> &parameters,
                             const wstring &upload_file,
                             const wstring &file_part_name,
                             int *timeout,
                             wstring *response_body,
                             int *response_code) {
  if (response_code) {
    *response_code = 0;
  }
  LOGDEBUG( "HTTPUpload::SendRequest");
  LOGDEBUG("\tURL: " << url.c_str());
  LOGDEBUG("\tUpload_File: " << upload_file.c_str());
  LOGDEBUG( "\tFile_part_name: " << file_part_name.c_str());
  LOGDEBUG( "\tParameters: ");
  string s;
  if (true) {
    for (std::map<wstring, wstring>::const_iterator it = parameters.begin();
      it != parameters.end(); ++it) {
        LOGDEBUG("\t\t" << it->first.c_str() << " = " << it->second.c_str());
    }
  }
  
  // TODO(bryner): support non-ASCII parameter names
  if (!CheckParameters(parameters)) {
    return false;
  }

  // Break up the URL and make sure we can handle it
  wchar_t scheme[16], host[256], path[256];
  URL_COMPONENTS components;
  memset(&components, 0, sizeof(components));
  components.dwStructSize = sizeof(components);
  components.lpszScheme = (LPSTR)scheme;
  components.dwSchemeLength = sizeof(scheme) / sizeof(scheme[0]);
  components.lpszHostName = (LPSTR)host;
  components.dwHostNameLength = sizeof(host) / sizeof(host[0]);
  components.lpszUrlPath = (LPSTR)path;
  components.dwUrlPathLength = sizeof(path) / sizeof(path[0]);
  if (!InternetCrackUrl((LPSTR)url.c_str(), static_cast<DWORD>(url.size()),
      0, &components)) {
    LOGERROR(  "InternetCrackUrl failed: " );
    LOGERROR(  "This indicates a malformed upload server name");
    return false;
  }
  bool secure = false;
  if (wcscmp(scheme, L"https") == 0) {
    secure = true;
  } else if (wcscmp(scheme, L"http") != 0) {
    return false;
  }

  AutoInternetHandle internet(InternetOpen((LPSTR)kUserAgent,
                                           INTERNET_OPEN_TYPE_PRECONFIG,
                                           NULL,  // proxy name
                                           NULL,  // proxy bypass
                                           0));   // flags
  if (!internet.get()) {
    LOGERROR( "InternetOpen returned NULL: " );
    return false;
  }

  AutoInternetHandle connection(InternetConnect((LPSTR)internet.get(),
                                                (LPSTR)host,
                                                components.nPort,
                                                NULL,    // user name
                                                NULL,    // password
                                                INTERNET_SERVICE_HTTP,
                                                0,       // flags
                                                NULL));  // context
  if (!connection.get()) {
    LOGERROR( "InternetConnect returned NULL: " );
    return false;
  }

  DWORD http_open_flags = secure ? INTERNET_FLAG_SECURE : 0;
  http_open_flags |= INTERNET_FLAG_NO_COOKIES;
  AutoInternetHandle request(HttpOpenRequest((LPSTR)connection.get(),
                                             (LPSTR)L"POST",
                                             (LPSTR)path,
                                             NULL,    // version
                                             NULL,    // referer
                                             NULL,    // agent type
                                             http_open_flags,
                                             NULL));  // context
  if (!request.get()) {
    LOGERROR( "HttpOpenRequest returned NULL: " );
    return false;
  }

  wstring boundary = GenerateMultipartBoundary();
  wstring content_type_header = GenerateRequestHeader(boundary);
  HttpAddRequestHeaders(request.get(),
                        (LPSTR)content_type_header.c_str(),
                        static_cast<DWORD>(-1),
                        HTTP_ADDREQ_FLAG_ADD);

  string request_body;
  if (!GenerateRequestBody(parameters, upload_file,
                           file_part_name, boundary, &request_body)) {
    return false;
  }

  if (timeout) {
    if (!InternetSetOption(request.get(),
                           INTERNET_OPTION_SEND_TIMEOUT,
                           timeout,
                           sizeof(timeout))) {
       LOGERROR("InternetSetOption on send timeout returned NULL: ");
       fwprintf(stderr, L"Could not unset send timeout, continuing...\n");
    }

    if (!InternetSetOption(request.get(),
                           INTERNET_OPTION_RECEIVE_TIMEOUT,
                           timeout,
                           sizeof(timeout))) {
      LOGERROR("InternetSetOption on receive timeout returned NULL: ");
      fwprintf(stderr, L"Could not unset receive timeout, continuing...\n");
    }
  }
  
  if (!HttpSendRequest(request.get(), NULL, 0,
                       const_cast<char *>(request_body.data()),
                       static_cast<DWORD>(request_body.size()))) {
    LOGERROR("HttpSendRequest on send returned NULL: ");
    return false;
  }

  // The server indicates a successful upload with HTTP status 200.
  wchar_t http_status[4];
  DWORD http_status_size = sizeof(http_status);
  if (!HttpQueryInfo(request.get(), HTTP_QUERY_STATUS_CODE,
                     static_cast<LPVOID>(&http_status), &http_status_size,
                     0)) {
    LOGERROR( "HttpQueryInfo after send returned NULL: ");
    return false;
  }

  int http_response = wcstol(http_status, NULL, 10);
  if (response_code) {
    *response_code = http_response;
  }

  bool result = (http_response == 200);

  if (result) {
    result = ReadResponse(request.get(), response_body);
  } else {
    LOGERROR("Http send request returned: " << result);
  }

  return result;
}

// static
bool StackDumpUploader::ReadResponse(HINTERNET request, wstring *response) {
  bool has_content_length_header = false;
  wchar_t content_length[32];
  DWORD content_length_size = sizeof(content_length);
  DWORD claimed_size = 0;
  string response_body;

  if (HttpQueryInfo(request, HTTP_QUERY_CONTENT_LENGTH,
                    static_cast<LPVOID>(&content_length),
                    &content_length_size, 0)) {
    has_content_length_header = true;
    claimed_size = wcstol(content_length, NULL, 10);
    response_body.reserve(claimed_size);
  }


  DWORD bytes_available;
  DWORD total_read = 0;
  BOOL return_code;

  while (((return_code = InternetQueryDataAvailable(request, &bytes_available,
	  0, 0)) != 0) && bytes_available > 0) {

    vector<char> response_buffer(bytes_available);
    DWORD size_read;

    return_code = InternetReadFile(request,
                                   &response_buffer[0],
                                   bytes_available, &size_read);

    if (return_code && size_read > 0) {
      total_read += size_read;
      response_body.append(&response_buffer[0], size_read);
    } else {
      break;
    }
  }

  bool succeeded = return_code && (!has_content_length_header ||
                                   (total_read == claimed_size));
  if (succeeded && response) {
    *response = UTF8ToWide(response_body);
  }

  return succeeded;
}

// static
wstring StackDumpUploader::GenerateMultipartBoundary() {
  // The boundary has 27 '-' characters followed by 16 hex digits
  static const wchar_t kBoundaryPrefix[] = L"---------------------------";
  static const int kBoundaryLength = 27 + 16 + 1;

  // Generate some random numbers to fill out the boundary
  int r0 = rand();
  int r1 = rand();

  wchar_t temp[kBoundaryLength];
  swprintf(temp, kBoundaryLength, L"%s%08X%08X", kBoundaryPrefix, r0, r1);

  // remove when VC++7.1 is no longer supported
  temp[kBoundaryLength - 1] = L'\0';

  return wstring(temp);
}

// static
wstring StackDumpUploader::GenerateRequestHeader(const wstring &boundary) {
  wstring header = L"Content-Type: multipart/form-data; boundary=";
  header += boundary;
  return header;
}

// static
bool StackDumpUploader::GenerateRequestBody(const map<wstring, wstring> &parameters,
                                     const wstring &upload_file,
                                     const wstring &file_part_name,
                                     const wstring &boundary,
                                     string *request_body) {
  vector<char> contents;
  GetFileContents(upload_file, &contents);
  if (contents.empty()) {
    return false;
  }

  string boundary_str = WideToUTF8(boundary);
  if (boundary_str.empty()) {
    return false;
  }

  request_body->clear();

  // Append each of the parameter pairs as a form-data part
  for (map<wstring, wstring>::const_iterator pos = parameters.begin();
       pos != parameters.end(); ++pos) {
    request_body->append("--" + boundary_str + "\r\n");
    request_body->append("Content-Disposition: form-data; name=\"" +
                         WideToUTF8(pos->first) + "\"\r\n\r\n" +
                         WideToUTF8(pos->second) + "\r\n");
  }

  // Now append the upload file as a binary (octet-stream) part
  string filename_utf8 = WideToUTF8(upload_file);
  if (filename_utf8.empty()) {
    return false;
  }

  string file_part_name_utf8 = WideToUTF8(file_part_name);
  if (file_part_name_utf8.empty()) {
    return false;
  }

  request_body->append("--" + boundary_str + "\r\n");
  request_body->append("Content-Disposition: form-data; "
                       "name=\"" + file_part_name_utf8 + "\"; "
                       "filename=\"" + filename_utf8 + "\"\r\n");
  request_body->append("Content-Type: application/octet-stream\r\n");
  request_body->append("\r\n");

  if (!contents.empty()) {
      request_body->append(&(contents[0]), contents.size());
  }
  request_body->append("\r\n");
  request_body->append("--" + boundary_str + "--\r\n");
  return true;
}

// static
void StackDumpUploader::GetFileContents(const wstring &filename,
                                 vector<char> *contents) {
  // The "open" method on pre-MSVC8 ifstream implementations doesn't accept a
  // wchar_t* filename, so use _wfopen directly in that case.  For VC8 and
  // later, _wfopen has been deprecated in favor of _wfopen_s, which does
  // not exist in earlier versions, so let the ifstream open the file itself.
#if _MSC_VER >= 1400  // MSVC 2005/8
  ifstream file;
  file.open(filename.c_str(), ios::binary);
#else  // _MSC_VER >= 1400
  ifstream file(_wfopen(filename.c_str(), L"rb"));
#endif  // _MSC_VER >= 1400
  if (file.is_open()) {
    file.seekg(0, ios::end);
    std::streamoff length = file.tellg();
    contents->resize(length);
    if (length != 0) {
        file.seekg(0, ios::beg);
        file.read(&((*contents)[0]), length);
    }
    file.close();
  } else {
    contents->clear();
  }
}

// static
wstring StackDumpUploader::UTF8ToWide(const string &utf8) {
  if (utf8.length() == 0) {
    return wstring();
  }

  // compute the length of the buffer we'll need
  int charcount = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);

  if (charcount == 0) {
    return wstring();
  }

  // convert
  wchar_t* buf = new wchar_t[charcount];
  MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, buf, charcount);
  wstring result(buf);
  delete[] buf;
  return result;
}

// static
string StackDumpUploader::WideToUTF8(const wstring &wide) {
  if (wide.length() == 0) {
    return string();
  }

  // compute the length of the buffer we'll need
  int charcount = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1,
                                      NULL, 0, NULL, NULL);
  if (charcount == 0) {
    return string();
  }

  // convert
  char *buf = new char[charcount];
  WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, buf, charcount,
                      NULL, NULL);

  string result(buf);
  delete[] buf;
  return result;
}

// static
bool StackDumpUploader::CheckParameters(const map<wstring, wstring> &parameters) {
  for (map<wstring, wstring>::const_iterator pos = parameters.begin();
       pos != parameters.end(); ++pos) {
    const wstring &str = pos->first;
    if (str.size() == 0) {
      LOGERROR( "Parameter " << str.c_str() << " had non ascii characters");
      return false;  // disallow empty parameter names
    }
    for (unsigned int i = 0; i < str.size(); ++i) {
      wchar_t c = str[i];
      if (c < 32 || c == '"' || c > 127) {
        LOGERROR("Parameter " << str.c_str() << " had non ascii characters");
        return false;
      }
    }
  }
  return true;
}

}
}
}
