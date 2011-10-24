#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(encodingtask_EXPORTS)
      #define ENCTASK_EXPORT __declspec( dllexport )
    #else
      #define ENCTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define ENCTASK_EXPORT
  #endif
#endif

