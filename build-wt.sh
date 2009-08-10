#!/bin/sh

TOPDIR=`pwd`
SRCDIR="$TOPDIR/source"
BUILDDIR="build"
mkdir -p $SRCDIR
. ./build-util.sh

download_file "http://dfn.dl.sourceforge.net/project/witty/wt/2.2.4/wt-2.2.4.tar.gz" "wt-2.2.4.tar.gz" "wt-2.2.4.tar"
gunzip_file "wt-2.2.4.tar.gz" "wt-2.2.4.tar"
untar_file "wt-2.2.4.tar" "wt-*" "wt"
cd $SRCDIR/wt
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$SRCDIR/$BUILDDIR/wt -DCMAKE_BUILD_TYPE=Release -DSHARED_LIBS=Off -DBOOST_DIR=/home/jhoelscher/MediaEncodingCluster/source/build/boost -DBOOST_VERSION=1_35 -DBOOST_COMPILER=gcc41 .. 
make 
make install
