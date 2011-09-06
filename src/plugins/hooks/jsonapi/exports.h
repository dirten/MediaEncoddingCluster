#ifndef ORG_ESB_API_EXPORTS
#define ORG_ESB_API_EXPORTS
  #if defined(_WIN32) && defined(API_BUILD_SHARED)
    #if defined(api_EXPORTS)
      #define API_EXPORT __declspec( dllexport )
    #else
      #define API_EXPORT __declspec( dllimport )
    #endif
  #else
    #define API_EXPORT
  #endif
#endif

