#ifndef ORG_ESB_HIVESERVICE_EXPORTS
#define ORG_ESB_JSONAPI_EXPORTS
  #if defined(_WIN32) 
    #if defined(hiveservice_EXPORTS)
      #define HIVESERVICE_EXPORT __declspec( dllexport )
    #else
      #define HIVESERVICE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define HIVESERVICE_EXPORT
  #endif
#endif

