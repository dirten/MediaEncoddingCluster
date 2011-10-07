#ifndef ORG_ESB_LOGSERVICE_EXPORTS
#define ORG_ESB_LOGSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(logserver_EXPORTS)
      #define LOGSERVICE_EXPORT __declspec( dllexport )
    #else
      #define LOGSERVICE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define LOGSERVICE_EXPORT
  #endif
#endif

