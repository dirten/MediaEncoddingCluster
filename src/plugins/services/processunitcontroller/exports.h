#ifndef ORG_ESB_PROCESSUNITCONTROLLER_EXPORTS
#define ORG_ESB_JOBCREATOR_EXPORTS
  #if defined(_WIN32) 
    #if defined(processunitcontroller_EXPORTS)
      #define PROCESSUNITCONTROLLER_EXPORT __declspec( dllexport )
    #else
      #define PROCESSUNITCONTROLLER_EXPORT __declspec( dllimport )
    #endif
  #else
    #define PROCESSUNITCONTROLLER_EXPORT
  #endif
#endif

