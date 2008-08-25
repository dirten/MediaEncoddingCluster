#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = hive
hive_SOURCES = Hive.cpp

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src

# the library search path.
hive_LDADD= -L./org/esb -llang -lesb  -lio  -lsql -lhive -lav -lutil -lsignal -lweb -lnet -lwt -lwtext -lwthttp -lboost_serialization
EXTRA_DIST=introspec.h import.cpp export.cpp job.cpp Environment.cpp CreateDatabase.cpp
