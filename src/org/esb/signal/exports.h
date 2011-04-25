#ifndef ORG_ESB_SIGNAL_EXPORTS
#define ORG_ESB_SIGNAL_EXPORTS
  #if defined(_WIN32) && defined(SIGNAL_BUILD_SHARED)
    #if defined(signal_EXPORTS)
      #define SIGNAL_EXPORT __declspec( dllexport )
	  #define EXPIMP_TEMPLATE
    #else
      #define SIGNAL_EXPORT __declspec( dllimport )
	  #define EXPIMP_TEMPLATE extern
    #endif
  #else
    #define SIGNAL_EXPORT
	#define EXPIMP_TEMPLATE
  #endif
#endif