#ifndef ORG_ESB_UTIL_EXPORTS
#define ORG_ESB_UTIL_EXPORTS
  #if defined(_WIN32)
    #if defined(util_EXPORTS)
      #define UTIL_EXPORT __declspec( dllexport )
    #else
      #define UTIL_EXPORT __declspec( dllimport )
    #endif
  #else
    #define UTIL_EXPORT
  #endif
#endif