#ifndef ORG_ESB_OUTPUTTASK_EXPORTS
#define ORG_ESB_OUTPUTTASK_EXPORTS
  #if defined(_WIN32)
    #if defined(outputtask_EXPORTS)
      #define OUTTASK_EXPORT __declspec( dllexport )
    #else
      #define OUTTASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define OUTTASK_EXPORT
  #endif
#endif

