include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)

#DIRS=\
#	rpc

TARG=ffmpeg

CGOFILES=ffmpeg.go Timestamp.go Rational.go Coder.go 
#Buffer.go
PROTOPATH=../../org/esb/rpc/
GOFILES=net.go 

CGO_LDFLAGS=-L/home/HoelscJ/devel/mec/target/dependency/lib/ -lavcodec -lavutil -lavformat -lavcore -lswscale
CGO_CFLAGS=-I/home/HoelscJ/devel/mec/target/dependency/include/

#include $(GOROOT)/src/pkg/goprotobuf.googlecode.com/hg/Make.protobuf

        
#OBJLIBS	= hivenet.a 

#all: deps

#clean: myclean

#myclean :
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
#	-for d in pkg; do (cd $$d; $(MAKE) clean ); done
	    

include $(GOROOT)/src/Make.pkg


