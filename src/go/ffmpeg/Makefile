include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)

#DIRS=\
#	rpc

TARG=gmf

CGOFILES=avformat.go avcodec.go avutil.go swscale.go
GOFILES=Datasource.go MediaLocator.go Demultiplexer.go Track.go Format.go Rational.go Timestamp.go Decoder.go Encoder.go Coder.go Datasink.go Multiplexer.go Resizer.go Resampler.go Deinterlacer.go FrameRateConverter.go gmf.go
#Buffer.go
#PROTOPATH=../../org/esb/rpc/
#GOFILES=net.go 

CGO_LDFLAGS=-L../../../target/dependency/lib/ -L../../../build/src/org/esb/ -lavcodec -lavutil -lavformat -lavcore -lswscale
CGO_CFLAGS=-I../../../target/dependency/include/


#include $(GOROOT)/src/pkg/goprotobuf.googlecode.com/hg/Make.protobuf

        
#OBJLIBS	= hivenet.a 

#all: deps

#clean: myclean

#myclean :
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
#	-for d in pkg; do (cd $$d; $(MAKE) clean ); done
	    

include $(GOROOT)/src/Make.pkg

# this hack is for the wrong handling of the cgo executable to generate the required header on windows
ifeq ($(GOOS),windows)
_cgo_import.c: _cgo1_.o
	cp cgo_import.orig.c _cgo_import.c
#	cgo -dynimport _cgo1_.o >_$@ && mv -f _$@ $@
endif



