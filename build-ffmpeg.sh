#!/bin/sh

TOPDIR=`pwd`
SRCDIR="$TOPDIR/source"
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
 	        "./configure" --prefix="$SRCDIR/$1-build" --disable-shared --enable-static $2 &> configure-$1.log
 	        cd "$TOPDIR"
 	}
configure_xvid(){
 	        cd "$SRCDIR/xvidcore/build/generic/"
 	        echo "Configuring xvidcore"
 	        "./configure" --prefix="$SRCDIR/xvidcore-build" $2 &> configure-xvidcore.log
 	        cd "$TOPDIR"
 	}
build_file(){
 	        cd "$SRCDIR/$1"
 	        echo "Building $1"
 	        make install&> make-$1.log
 	        if test $? -ne 0; then
 	                echo "make failed - log available: $1/make-$1.log"
 	                exit 1
 	        fi
 	        cd "$TOPDIR"
 	}
build_xvid(){
 	        cd "$SRCDIR/xvidcore/build/generic"
 	        echo "Building xvidcore"
 	        make &> make.log
 	        if test $? -ne 0; then
 	                echo "make failed - log available: xvidcore/make-xvidcore.log"
 	                exit 1
 	        fi
 	        make install&> make.log
 	        cd "$TOPDIR"
 	}
download_file "http://ffmpeg.org/releases/ffmpeg-export-snapshot.tar.bz2" "ffmpeg-export-snapshot.tar.bz2" "ffmpeg-export-snapshot.tar" 
download_file "http://download.videolan.org/pub/videolan/x264/snapshots/x264-snapshot-20090524-2245.tar.bz2" "x264-snapshot-20090524-2245.tar.bz2" "x264-snapshot-20090524-2245.tar"
download_file "http://dfn.dl.sourceforge.net/sourceforge/lame/lame-398-2.tar.gz" "lame.src.tar.gz" "lame.src.tar"
download_file "http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz" "libogg-1.1.3.tar.gz" "libogg-1.1.3.tar"
download_file "http://downloads.xiph.org/releases/theora/libtheora-1.0.tar.bz2" "libtheora-1.0.tar.bz2" "libtheora-1.0.tar"
download_file "http://downloads.xiph.org/releases/speex/speex-1.0.5.tar.gz" "speex-1.0.5.tar.gz" "speex-1.0.5.tar"
download_file "http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz" "libvorbis-1.2.0.tar.gz" "libvorbis-1.2.0.tar"
download_file "http://downloads.xvid.org/downloads/xvidcore-1.2.1.tar.gz" "xvidcore-1.2.1.tar.gz" "xvidcore-1.2.1.tar"

bunzip_file "ffmpeg-export-snapshot.tar.bz2"
bunzip_file "x264-snapshot-20090524-2245.tar.bz2"
gunzip_file "lame.src.tar.gz"
gunzip_file "libogg-1.1.3.tar.gz"
bunzip_file "libtheora-1.0.tar.bz2"
gunzip_file "speex-1.0.5.tar.gz"
gunzip_file "libvorbis-1.2.0.tar.gz"
gunzip_file "xvidcore-1.2.1.tar.gz"

untar_file "ffmpeg-export-snapshot.tar" "ffmpeg-export*" "ffmpeg"
untar_file "x264-snapshot-20090524-2245.tar" "x264-snap*" "x264"
untar_file "lame.src.tar" "lame-*" "lame"
untar_file "libogg-1.1.3.tar" "libogg-*" "libogg"
untar_file "libtheora-1.0.tar" "libtheora-*" "libtheora"
untar_file "speex-1.0.5.tar" "speex-*" "speex"
untar_file "libvorbis-1.2.0.tar" "libvorbis-*" "libvorbis"
untar_file "xvidcore-1.2.1.tar" "xvidcore" ""

configure_file "lame"
build_file "lame"
configure_file "x264" "--disable-asm"
build_file "x264"
configure_file "libogg"
build_file "libogg"
export LD_LIBRARY_PATH=$SRCDIR/libogg-build/lib
configure_file "libtheora" "--with-ogg=$SRCDIR/libogg-build"
build_file "libtheora"
configure_file "speex" "--with-ogg-dir=$SRCDIR/libogg-build"
build_file "speex"
configure_file "libvorbis" "--with-ogg=$SRCDIR/libogg-build"
build_file "libvorbis"
configure_xvid
build_xvid

SYS=`uname`
if [[ $SYS != MINGW32* ]]; then
  LIBPTHREAD="-extra-ldflags=-lpthread"
fi

configure_file "ffmpeg" \
"--enable-libxvid --extra-cflags=-I$SRCDIR/xvidcore-build/include --extra-ldflags=-L$SRCDIR/xvidcore-build/lib \
--enable-libx264 --extra-cflags=-I$SRCDIR/x264-build/include --extra-ldflags=-L$SRCDIR/x264-build/lib --enable-gpl \
--enable-libmp3lame --extra-cflags=-I$SRCDIR/lame-build/include --extra-ldflags=-L$SRCDIR/lame-build/lib  \
--enable-libvorbis --extra-cflags=-I$SRCDIR/libvorbis-build/include --extra-ldflags=-L$SRCDIR/libvorbis-build/lib \
--enable-libtheora --extra-cflags=-I$SRCDIR/libtheora-build/include --extra-ldflags=-L$SRCDIR/libtheora-build/lib \
--extra-ldflags=-L$SRCDIR/libogg-build/lib \
$LIBPTHREAD --extra-cflags=-I$SRCDIR/libogg-build/include --disable-devices --enable-memalign-hack"
#--extra-ldflags=-lpthread 
#--enable-libspeex --extra-cflags=-I$SRCDIR/speex-build/include --extra-ldflags=-L$SRCDIR/speex-build/lib \

build_file "ffmpeg"
