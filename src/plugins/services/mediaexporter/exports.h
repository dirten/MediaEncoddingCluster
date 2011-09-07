#ifndef ORG_ESB_MEDIAEXPORTER_EXPORTS
#define ORG_ESB_MEDIAEXPORTER_EXPORTS
  #if defined(_WIN32) 
    #if defined(mediaexporter_EXPORTS)
      #define MEDIAEXPORTER_EXPORT __declspec( dllexport )
    #else
      #define MEDIAEXPORTER_EXPORT __declspec( dllimport )
    #endif
  #else
    #define MEDIAEXPORTER_EXPORT
  #endif
#endif

