#ifndef ORG_ESB_JSONAPI_EXPORTS
#define ORG_ESB_JSONAPI_EXPORTS
  #if defined(_WIN32) 
    #if defined(jsonapi_EXPORTS)
      #define JSONAPI_EXPORT __declspec( dllexport )
    #else
      #define JSONAPI_EXPORT __declspec( dllimport )
    #endif
  #else
    #define JSONAPI_EXPORT
  #endif
#endif

