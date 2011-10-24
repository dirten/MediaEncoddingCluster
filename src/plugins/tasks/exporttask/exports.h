#ifndef ORG_ESB_WEBSERVICE_EXPORTS
#define ORG_ESB_WEBSERVICE_EXPORTS
  #if defined(_WIN32)
    #if defined(exporttask_EXPORTS)
      #define EXPTASK_EXPORT __declspec( dllexport )
    #else
      #define EXPTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define EXPTASK_EXPORT
  #endif
#endif

