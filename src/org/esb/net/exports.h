#ifndef ORG_ESB_NET_EXPORTS
#define ORG_ESB_NET_EXPORTS
  #if defined(_WIN32) && defined(SHARED)
    #if defined(net_EXPORTS)
      #define NET_EXPORT __declspec( dllexport )
    #else
      #define NET_EXPORT __declspec( dllimport )
    #endif
  #else
    #define NET_EXPORT
  #endif
#endif

