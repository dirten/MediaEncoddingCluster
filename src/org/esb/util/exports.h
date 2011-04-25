#ifndef ORG_ESB_UTIL_EXPORTS
#define ORG_ESB_UTIL_EXPORTS
  #if defined(_WIN32)
    #if defined(util_EXPORTS)
      #define UTIL_EXPORT __declspec( dllexport )
	  #define EXPIMP_TEMPLATE
    #else
      #define UTIL_EXPORT __declspec( dllimport )
	  #define EXPIMP_TEMPLATE extern
    #endif
  #else
    #define UTIL_EXPORT
	#define EXPIMP_TEMPLATE
  #endif
#endif