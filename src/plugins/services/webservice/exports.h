#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(webservice_EXPORTS)
      #define WEBSERVICE_EXPORT __declspec( dllexport )
    #else
      #define WEBSERVICE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define WEBSERVICE_EXPORT
  #endif
#endif

