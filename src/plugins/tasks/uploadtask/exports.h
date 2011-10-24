#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(uploadtask_EXPORTS)
      #define UPTASK_EXPORT __declspec( dllexport )
    #else
      #define UPTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define UPTASK_EXPORT
  #endif
#endif

