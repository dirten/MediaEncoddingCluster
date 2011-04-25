include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)

#DIRS=\
#	rpc

TARG=test

CGOFILES=Test.go
#Buffer.go
#PROTOPATH=../../org/esb/rpc/
#GOFILES=net.go 

CGO_LDFLAGS=-L../../../target/dependency/lib/ -lavcodec -lavutil -lavformat -lavcore -lswscale
CGO_CFLAGS=-I../../../target/dependency/include/

#include $(GOROOT)/src/pkg/goprotobuf.googlecode.com/hg/Make.protobuf

        
#OBJLIBS	= hivenet.a 

#all: deps

#clean: myclean

#myclean :
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
#	-for d in pkg; do (cd $$d; $(MAKE) clean ); done
	    
%.c:	%.goc goc2c
	goc2c `pwd`/$< > $@.tmp
	mv -f $@.tmp $@

include $(GOROOT)/src/Make.pkg
# this hack is for the wrong handling of the cgo executable to #generate the required header on windows
#ifeq ($(GOOS),windows)
_cgo_import.c: _cgo1_.o
	cp cgo_import.orig.c _cgo_import.c
#_cgo_import.c: _cgo1_.o
#	cgo -dynimport _cgo1_.o >_$@ && mv -f _$@ $@
#	echo "#pragma dynimport av_audio_resample_init av_audio_resample_init \"avcodec.dll\"\r\n" >> _cgo_import.c
#	echo "#pragma dynimport av_register_all av_register_all \"avformat.dll\"\r\n" >> _cgo_import.c
#endif


