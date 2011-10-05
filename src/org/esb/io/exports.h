#ifndef ORG_ESB_IO_EXPORTS
#define ORG_ESB_IO_EXPORTS
  #if defined(_WIN32)
    #if defined(io_EXPORTS)
      #define IO_EXPORT __declspec( dllexport )
    #else
      #define IO_EXPORT __declspec( dllimport )
    #endif
  #else
    #define IO_EXPORT
  #endif
#endif

