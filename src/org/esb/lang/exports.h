#ifndef ORG_ESB_LANG_EXPORTS
#define ORG_ESB_LANG_EXPORTS
  #if defined(_WIN32) && defined(SHARED)
    #if defined(lang_EXPORTS)
      #define LANG_EXPORT __declspec( dllexport )
    #else
      #define LANG_EXPORT __declspec( dllimport )
    #endif
  #else
    #define LANG_EXPORT
  #endif
#endif

