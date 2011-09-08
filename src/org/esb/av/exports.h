#ifndef ORG_ESB_AV_EXPORTS
#define ORG_ESB_AV_EXPORTS
  #if defined(_WIN32) 
    #if defined(av_EXPORTS)
      #define AV_EXPORT __declspec( dllexport )
    #else
      #define AV_EXPORT __declspec( dllimport )
    #endif
  #else
    #define AV_EXPORT
  #endif
#endif

