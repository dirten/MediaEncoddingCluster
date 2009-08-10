#!/bin/sh



TOPDIR=`pwd`
SRCDIR="$TOPDIR/source"
BUILDDIR="build"
mkdir -p $SRCDIR
. ./build-util.sh

download_file "http://dfn.dl.sourceforge.net/project/boost/boost/1.35.0/boost_1_35_0.tar.bz2" "boost.src.tar.bz2" "boost.src.tar"
bunzip_file "boost.src.tar.bz2" "boost.src.tar"
untar_file "boost.src.tar" "boost_*" "boost"
if test ! -f source/boost/tools/jam/stage/bin.linuxx86/bjam ; then
  echo "Build Jam"
  cd source/boost/tools/jam
  ./build_dist.sh
fi
echo "Build Boost"
cd $SRCDIR/boost
tools/jam/stage/bin.linuxx86/bjam --prefix=$SRCDIR/build/boost --without-python --without-wave --without-graph --without-test --without-mpi variant=release link=static threading=multi install
