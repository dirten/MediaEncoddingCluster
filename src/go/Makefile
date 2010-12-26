include $(GOROOT)/src/Make.inc
TARG=mainrpc

DIRS=\
	pkg

GOFILES=\
        mainrpc.go
        
        
#OBJLIBS	= hivenet.a 

all: deps

clean: myclean


deps : 
	cd pkg; $(MAKE) $(MFLAGS); $(MAKE) install

myclean :
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	-for d in pkg; do (cd $$d; $(MAKE) clean ); done
	    

include $(GOROOT)/src/Make.cmd


