#ifndef ORG_ESB_MEDIAIMPORTER_EXPORTS
#define ORG_ESB_MEDIAIMPORTER_EXPORTS
  #if defined(_WIN32) 
    #if defined(mediaimporter_EXPORTS)
      #define MEDIAIMPORTER_EXPORT __declspec( dllexport )
    #else
      #define MEDIAIMPORTER_EXPORT __declspec( dllimport )
    #endif
  #else
    #define MEDIAIMPORTER_EXPORT
  #endif
#endif

