#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(downloadtask_EXPORTS)
      #define DLTASK_EXPORT __declspec( dllexport )
    #else
      #define DLTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define DLTASK_EXPORT
  #endif
#endif

