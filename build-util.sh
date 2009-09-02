download_file(){
  cd "$SRCDIR"
  if test ! -f $3 && test ! -f $2; then
    echo "Downloading $1"
    curl  -L "$1" -o $2
    if test ! -f $2 ; then
      echo "Could not download $2"
 	    exit 1
 	  fi
 	else
    echo "Found $2 in the srcdir $SRCDIR"
 	fi
 	cd "$TOPDIR"
}


gunzip_file(){
#  unzip_file $1 $2 gunzip
  cd "$SRCDIR"
  
  if test -f $1 && test ! -f $2; then
    echo "gunzip $1"
    gunzip  "$1"
  fi 
  cd "$TOPDIR"
}

bunzip_file(){
#  unzip_file $1 $2 bunzip2
  cd "$SRCDIR"
  if test -f $1 && test ! -f $2; then
    echo "bunzip2 $1"
    bunzip2  "$1"
  fi 
  cd "$TOPDIR"
}

untar_file(){
  cd "$SRCDIR"
  if test -f $1 && test ! -L $3 ; then
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

rename_file(){
  cd $SRCDIR/$BUILDDIR/$1
  if test -f $2.a ; then
    mv $2.a $2.lib
  fi
  cd "$TOPDIR"
}
