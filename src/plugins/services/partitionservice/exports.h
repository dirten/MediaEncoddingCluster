#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(partitionservice_EXPORTS)
      #define PARTSERVICE_EXPORT __declspec( dllexport )
    #else
      #define PARTSERVICE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define PARTSERVICE_EXPORT
  #endif
#endif

