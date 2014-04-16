#OCLSDKDIR = /usr/local/cuda
OCLSDKDIR = ${AMDAPPSDKROOT}
CPP = g++
OCLSDKINC = ${OCLSDKDIR}/include
OCLSDKLIB = ${OCLSDKDIR}/lib/x86
OPTFLAG = -O2 -fomit-frame-pointer
INCLUDES = -I../common/inc
FLAGS = ${OPTFLAG} ${INCLUDES} -I${OCLSDKINC} -msse -msse2
LFLAGS = -L${OCLSDKLIB}
LIBPARS = -lOpenCL -lrt

oclfactor8to1: oclfactor8to1.o
	${CPP} ${LFLAGS} -o $@ $< ${LIBPARS}

oclfactor8to1.o: oclfactor8to1.cpp
	${CPP} -c ${FLAGS} $<

clean:
	rm oclfactor8to1 oclfactor8to1.o
