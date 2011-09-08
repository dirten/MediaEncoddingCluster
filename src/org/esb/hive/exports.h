#ifndef ORG_ESB_HIVE_EXPORTS
#define ORG_ESB_HIVE_EXPORTS
  #if defined(_WIN32) 
    #if defined(hive_EXPORTS)
      #define HIVE_EXPORT __declspec( dllexport )
    #else
      #define HIVE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define HIVE_EXPORT
  #endif
#endif

