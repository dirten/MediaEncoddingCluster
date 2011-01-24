include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)


TARG=testffmpeg
#CGOFILES=ffmpegtest.go
#CGO_LDFLAGS=-L/home/HoelscJ/devel/mec/target/dependency/lib/ -lavcodec -lavutil -lavformat -lavcore -lswscale
#CGO_CFLAGS=-I/home/HoelscJ/devel/mec/target/dependency/include/

DIRS=\
	pkg

GOFILES=\
        mainffmpeg.go \
        main.go
        
        
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


