#!/bin/sh



X264_VERSION="-snapshot-20090611-2245"

DOWNLOAD_LIBS="yes"


TOPDIR=`pwd`
SRCDIR="$TOPDIR/source"
BUILDDIR="build"
mkdir -p $SRCDIR
download_file(){
  cd "$SRCDIR"
  if test ! -f $3 ; then
    echo "Downloading $1"
    curl  "$1" -o $2
    if test ! -f $2 ; then
      echo "Could not download $2"
 	    exit 1
 	  fi
 	else
    echo "Found $2 in the srcdir $SRCDIR"
 	fi
 	cd "$TOPDIR"
}

bunzip_file(){
  cd "$SRCDIR"
  if test -f $1 ; then
    echo "bunzip2 $1"
    bunzip2  "$1"
  fi 
}
gunzip_file(){
  cd "$SRCDIR"
  if test -f $1 ; then
    echo "gunzip $1"
    gunzip  "$1"
  fi 
  cd "$TOPDIR"
}

untar_file(){
  cd "$SRCDIR"
  if test -f $1 ; then
    echo "UnTar $1"
    tar -xf  "$1"
    DIR=`basename $2`
    ln -s $DIR $3
    echo $DIR
  fi 
  cd "$TOPDIR"
}
configure_file(){
 	        cd "$SRCDIR/$1"
 	        echo "Configuring $1"
 	        "./configure" --prefix="$SRCDIR/$BUILDDIR/$1" --enable-shared --disable-static $2 > configure-$1.log
 	        cd "$TOPDIR"
 	}
configure_xvid(){
 	        cd "$SRCDIR/xvidcore/build/generic/"
 	        echo "Configuring xvidcore"
 	        "./configure" --prefix="$SRCDIR/$BUILDDIR/xvidcore" $2 > configure-xvidcore.log
 	        cd "$TOPDIR"
 	}
build_file(){
 	        cd "$SRCDIR/$1"
 	        echo "Building $1"
 	        make install > make-$1.log
 	        if test $? -ne 0; then
 	                echo "make failed - log available: $1/make-$1.log"
 	                exit 1
 	        fi
 	        cd "$TOPDIR"
 	}
build_xvid(){
 	        cd "$SRCDIR/xvidcore/build/generic"
 	        echo "Building xvidcore"
 	        make > make.log
 	        if test $? -ne 0; then
 	                echo "make failed - log available: xvidcore/make-xvidcore.log"
 	                exit 1
 	        fi
 	        make install > make.log
 	        cd "$TOPDIR"
 	}
rename_file(){
  cd $SRCDIR/$BUILDDIR/$1
  if test -f $2.a ; then
    mv $2.a $2.lib
  fi
  cd "$TOPDIR"
}

download_file "http://www.nasm.us/pub/nasm/releasebuilds/2.06rc16/nasm-2.06rc16.tar.bz2" "nasm.tar.bz2" "nasm.tar"
download_file "http://www.tortall.net/projects/yasm/releases/yasm-0.8.0.tar.gz" "yasm.tar.gz" "yasm.tar"
download_file "http://ffmpeg.org/releases/ffmpeg-export-snapshot.tar.bz2" "ffmpeg-export-snapshot.tar.bz2" "ffmpeg-export-snapshot.tar" 
download_file "http://download.videolan.org/pub/videolan/x264/snapshots/x264$X264_VERSION.tar.bz2" "x264-snapshot.tar.bz2" "x264-snapshot.tar"
download_file "http://dfn.dl.sourceforge.net/sourceforge/lame/lame-398-2.tar.gz" "lame.src.tar.gz" "lame.src.tar"
download_file "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz" "libogg-1.1.3.tar.gz" "libogg-1.1.3.tar"
download_file "http://downloads.xiph.org/releases/theora/libtheora-1.0.tar.bz2" "libtheora-1.0.tar.bz2" "libtheora-1.0.tar"
download_file "http://downloads.xiph.org/releases/speex/speex-1.0.5.tar.gz" "speex-1.0.5.tar.gz" "speex-1.0.5.tar"
download_file "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz" "libvorbis-1.2.0.tar.gz" "libvorbis-1.2.0.tar"
download_file "http://downloads.xvid.org/downloads/xvidcore-1.2.1.tar.gz" "xvidcore-1.2.1.tar.gz" "xvidcore-1.2.1.tar"

gunzip_file "yasm.tar.gz"
bunzip_file "nasm.tar.bz2"
bunzip_file "ffmpeg-export-snapshot.tar.bz2"
bunzip_file "x264-snapshot.tar.bz2"
gunzip_file "lame.src.tar.gz"
gunzip_file "libogg-1.1.3.tar.gz"
bunzip_file "libtheora-1.0.tar.bz2"
gunzip_file "speex-1.0.5.tar.gz"
gunzip_file "libvorbis-1.2.0.tar.gz"
gunzip_file "xvidcore-1.2.1.tar.gz"

untar_file "nasm.tar" "nasm-*" "nasm"
untar_file "yasm.tar" "yasm-*" "yasm"
untar_file "ffmpeg-export-snapshot.tar" "ffmpeg-export*" "ffmpeg"
untar_file "x264-snapshot.tar" "x264-snap*" "x264"
untar_file "lame.src.tar" "lame-*" "lame"
untar_file "libogg-1.1.3.tar" "libogg-*" "libogg"
untar_file "libtheora-1.0.tar" "libtheora-*" "libtheora"
untar_file "speex-1.0.5.tar" "speex-*" "speex"
untar_file "libvorbis-1.2.0.tar" "libvorbis-*" "libvorbis"
untar_file "xvidcore-1.2.1.tar" "xvidcore" ""

export LD_LIBRARY_PATH=$SRCDIR/libogg-build/lib

configure_file "nasm"
build_file "nasm"

export PATH=/home/framebuster/bruteripper/source/build/nasm/bin:$PATH

configure_file "yasm"
build_file "yasm"

export PATH=/home/framebuster/bruteripper/source/build/yasm/bin:$PATH

configure_file "lame"
build_file "lame"
configure_file "x264" "--enable-debug"
build_file "x264"
configure_file "libogg"
build_file "libogg"
configure_file "libtheora" "--with-ogg=$SRCDIR/$BUILDDIR/libogg"
build_file "libtheora"
#configure_file "speex" "--with-ogg-dir=$SRCDIR/$BUILDDIR/libogg"
#build_file "speex"
configure_file "libvorbis" "--with-ogg=$SRCDIR/$BUILDDIR/libogg"
build_file "libvorbis"
configure_xvid
build_xvid

SYS=`uname`
case "$SYS" in
    MINGW32*)
#	rm -f $SRCDIR/$BUILDDIR/xvidcore/lib/xvidcore.dll
      MEMALIGNHACK="--enable-memalign-hack"
    ;;
    *)
    LIBPTHREAD="--extra-ldflags=-lpthread"
    ;;
esac

#if [ $SYS != MINGW32* ]; then
#  LIBPTHREAD="--extra-ldflags=-lpthread"
#fi

configure_file "ffmpeg" \
"--enable-libxvid --extra-cflags=-I$SRCDIR/$BUILDDIR/xvidcore/include --extra-ldflags=-L$SRCDIR/$BUILDDIR/xvidcore/lib \
--enable-libx264 --extra-cflags=-I$SRCDIR/$BUILDDIR/x264/include --extra-ldflags=-L$SRCDIR/$BUILDDIR/x264/lib --enable-gpl \
--enable-libmp3lame --extra-cflags=-I$SRCDIR/$BUILDDIR/lame/include --extra-ldflags=-L$SRCDIR/$BUILDDIR/lame/lib  \
--enable-libvorbis --extra-cflags=-I$SRCDIR/$BUILDDIR/libvorbis/include --extra-ldflags=-L$SRCDIR/$BUILDDIR/libvorbis/lib \
--enable-libtheora --extra-cflags=-I$SRCDIR/$BUILDDIR/libtheora/include --extra-ldflags=-L$SRCDIR/$BUILDDIR/libtheora/lib \
--extra-ldflags=-L$SRCDIR/$BUILDDIR/libogg/lib \
$LIBPTHREAD --extra-cflags=-I$SRCDIR/$BUILDDIR/libogg/include --disable-devices $MEMALIGNHACK --extra-cflags=-fno-common --disable-stripping"

build_file "ffmpeg"




case "$SYS" in
    MINGW32*)
      echo "Fixing mingw Build"
  echo "Copy required Libraries from MinGW"
  cd $SRCDIR/$BUILDDIR/ffmpeg/lib
  cp /usr/mingw/lib/libmingwex.a libmingwex.lib
  cp /usr/mingw/lib/libcoldname.a libcoldname.lib
  cp /usr/mingw/lib/libmoldname90.a libmoldname90.lib
  cp /usr/mingw/lib/gcc/mingw32/4.2.1-sjlj/libgcc.a libgcc.lib
  echo "Remove Symbols from libmingwex.lib"
  lib -remove:mbrtowc.o libmingwex.lib
  lib -remove:wcrtomb.o libmingwex.lib
  echo "rename builded Libraries"
  rename_file "ffmpeg/lib" "libavformat"
  rename_file "ffmpeg/lib" "libavcodec"
  rename_file "ffmpeg/lib" "libavdevice"
  rename_file "ffmpeg/lib" "libavutil"
  rename_file "ffmpeg/lib" "libswscale"
  rename_file "lame/lib" "libmp3lame"
  rename_file "libogg/lib" "libogg"
  rename_file "libtheora/lib" "libtheora"
  rename_file "libvorbis/lib" "libvorbis"
  rename_file "libvorbis/lib" "libvorbisenc"
  rename_file "x264/lib" "libx264"
  rename_file "xvidcore/lib" "xvidcore"

    ;;
    *)
    LIBPTHREAD="--extra-ldflags=-lpthread"
    ;;
esac

      echo "Build successfull"
