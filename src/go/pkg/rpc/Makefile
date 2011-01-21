include $(GOROOT)/src/Make.inc

PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)


TARG=org.esb.rpc

#PROTOPATH=../../../org/esb/rpc/
PROTOPATH=.
GOFILES=rpc.pb.go

include $(GOROOT)/src/pkg/goprotobuf.googlecode.com/hg/Make.protobuf

        
#OBJLIBS	= hivenet.a 

#all: deps

#clean: myclean

#myclean :
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
#	-for d in pkg; do (cd $$d; $(MAKE) clean ); done
	    

include $(GOROOT)/src/Make.pkg


