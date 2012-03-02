#ifndef ORG_ESB_MQSERVICE_EXPORTS
#define ORG_ESB_MQSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(mqserver_EXPORTS)
      #define MQSERVICE_EXPORT __declspec( dllexport )
    #else
      #define MQSERVICE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define MQSERVICE_EXPORT
  #endif
#endif

