#ifndef ORG_ESB_PARTSERVICE_EXPORTS
#define ORG_ESB_PARTSERVICE_EXPORTS
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

