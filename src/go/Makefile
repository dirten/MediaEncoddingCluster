include $(GOROOT)/src/Make.inc
PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)


TARG=dbtest
#CGOFILES=ffmpegtest.go
#CGO_LDFLAGS=-L/home/HoelscJ/devel/mec/target/dependency/lib/ -lavcodec -lavutil -lavformat -lavcore -lswscale
#CGO_CFLAGS=-I/home/HoelscJ/devel/mec/target/dependency/include/

DIRS=\
	db

GOFILES=testdb.go
CGO_LDFLAGS=-L/home/jan.hoelscher/devel/mec/target/dependency/lib -lsqlite3 -lpthread -ldl

#        transcoder.go
#	alloc_data.go
        
#        mainffmpeg.go 
        
#OBJLIBS	= hivenet.a 

all: deps

clean: myclean


deps : 
	for dir in $(DIRS); do (cd $$dir; $(MAKE) install ); done 
#	cd pkg; $(MAKE) $(MFLAGS); $(MAKE) install \
#	cd ffmpeg; $(MAKE) $(MFLAGS); $(MAKE) install


myclean :
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done 
#	echo cleaning up in .
#	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
#	for d in pkg; do (cd $$d; $(MAKE) clean ); done 
#	for d in ffmpeg; do (cd $$d; $(MAKE) clean ); done
test :
	for d in $(DIRS); do (cd $$d; $(MAKE) test ); done 

include $(GOROOT)/src/Make.cmd


