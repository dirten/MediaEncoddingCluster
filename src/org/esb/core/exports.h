#ifndef ORG_ESB_CORE_EXPORTS
#define ORG_ESB_CORE_EXPORTS
  #if defined(_WIN32) && defined(CORE_BUILD_SHARED)
    #if defined(core_EXPORTS)
      #define CORE_EXPORT __declspec( dllexport )
    #else
      #define CORE_EXPORT __declspec( dllimport )
    #endif
  #else
    #define CORE_EXPORT
  #endif
#endif

