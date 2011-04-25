#ifndef ORG_ESB_DB_EXPORTS
#define ORG_ESB_DB_EXPORTS
  #if defined(_WIN32)
    #if defined(db_EXPORTS)
      #define DB_EXPORT __declspec( dllexport )
    #else
      #define DB_EXPORT __declspec( dllimport )
    #endif
  #else
    #define DB_EXPORT
  #endif
#endif