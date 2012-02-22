#ifndef HTTP_PULL_SOURCE_EXPORTS
#define HTTP_PULL_SOURCE_EXPORTS
  #if defined(_WIN32)
    #if defined(httppullsource_EXPORTS)
      #define HTTPPULLSOURCETASK_EXPORT __declspec( dllexport )
    #else
      #define HTTPPULLSOURCETASK_EXPORT __declspec( dllimport )
    #endif
  #else
    #define HTTPPULLSOURCETASK_EXPORT
  #endif
#endif

