#ifndef ORG_ESB_EXECTASK_EXPORTS
#define ORG_ESB_EXECTASK_EXPORTS
  #if defined(_WIN32)
    #if defined(executabletask_EXPORTS)
      #define EXECTASK_EXPORT __declspec( dllexport )
    #else
      #define EXECTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define EXECTASK_EXPORT
  #endif
#endif

