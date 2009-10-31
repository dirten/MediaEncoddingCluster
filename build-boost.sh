#!/bin/sh



TOPDIR=`pwd`
SRCDIR="$TOPDIR/source"
BUILDDIR="build"
mkdir -p $SRCDIR
. ./build-util.sh
echo dirname $0

SYS=`uname`
case "$SYS" in
    Linux*)
      JAMBIN="linux"
    ;;
    Darwin*)
      JAMBIN="macosx"
    ;;
    *)
    
    ;;
esac

download_file "http://dfn.dl.sourceforge.net/project/boost/boost/1.36.0/boost_1_36_0.tar.bz2" "boost.src.tar.bz2" "boost.src.tar"
bunzip_file "boost.src.tar.bz2" "boost.src.tar"
untar_file "boost.src.tar" "boost_*" "boost"
if test ! -f source/boost/tools/jam/stage/bin.${JAMBIN}x86/bjam ; then
  echo "Build Jam"
  cd source/boost/tools/jam
  ./build_dist.sh
fi
echo "Build Boost"
cd $SRCDIR/boost
tools/jam/stage/bin.${JAMBIN}x86/bjam --prefix=$SRCDIR/build/boost --layout=system --without-python --without-wave --without-graph --without-test --without-mpi variant=release link=static threading=multi install
