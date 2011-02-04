include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)

#DIRS=\
#	rpc

TARG=test

CGOFILES=Test.go
#Buffer.go
#PROTOPATH=../../org/esb/rpc/
#GOFILES=net.go 

#CGO_LDFLAGS=-L../../../target/dependency/lib/ -L../../../build/src/org/esb/ -lavcodec -lavutil -lavformat -lavcore -lswscale
#CGO_CFLAGS=-I../../../target/dependency/include/

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
include $(GOROOT)/src/Make.clib


