#ifndef ORG_ESB_SQL_MYSQL_H
#define ORG_ESB_SQL_MYSQL_H
//#pragma warning( disable : 4005 )
#include <mysql_version.h>
#ifdef WIN32 
#include <winsock.h>
#if MYSQL_VERSION_ID < 50100
#include <my_global.h>
#endif
#endif
#include <mysql.h>

#endif
