#ifndef ORG_ESB_UDPNODERSOLVER_EXPORTS
#define ORG_ESB_UDPNODERSOLVER_EXPORTS
  #if defined(_WIN32)
    #if defined(udpnoderesolver_EXPORTS)
      #define UDPNODERSOLVER_EXPORT __declspec( dllexport )
    #else
      #define UDPNODERSOLVER_EXPORT __declspec( dllimport )
    #endif
  #else
    #define UDPNODERSOLVER_EXPORT
  #endif
#endif

