#ifndef ORG_ESB_JOBCREATOR_EXPORTS
#define ORG_ESB_JOBCREATOR_EXPORTS
  #if defined(_WIN32) 
    #if defined(jobcreator_EXPORTS)
      #define JOBCREATOR_EXPORT __declspec( dllexport )
    #else
      #define JOBCREATOR_EXPORT __declspec( dllimport )
    #endif
  #else
    #define JOBCREATOR_EXPORT
  #endif
#endif

