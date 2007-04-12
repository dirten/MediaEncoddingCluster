#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
bin_PROGRAMS = framesender zpipe
#bin_PROGRAMS = zpipe
framesender_SOURCES = bruteripper.cpp FrameContainer.cpp FrameHive.cpp FrameCompressor.cpp minilzo.c
zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
bruteripper_LDFLAGS = $(all_libraries) 
zpipe_LDFLAGS = $(all_libraries) 
noinst_HEADERS=*.h