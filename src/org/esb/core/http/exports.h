#ifndef ORG_ESB_CORE_HTTP_EXPORTS
#define ORG_ESB_CORE_HTTP_EXPORTS
  #if defined(_WIN32) 
    #if defined(core_http_EXPORTS)
      #define CORE_HTTP_EXPORT __declspec( dllexport )
    #else
      #define CORE_HTTP_EXPORT __declspec( dllimport )
    #endif
  #else
    #define CORE_HTTP_EXPORT
  #endif
#endif

