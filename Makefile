OCLSDKDIR = /usr/local/cuda
CPP = g++
OCLSDKINC = ${OCLSDKDIR}/include
OCLSDKLIB = ${OCLSDKDIR}/lib64
OPTFLAG = -O2 -fomit-frame-pointer
INCLUDES = -I../common/inc
FLAGS = ${OPTFLAG} ${INCLUDES} -I${OCLSDKINC} -msse -msse2 -msse3 -mavx -fopt-info-vec-missed
LFLAGS = -L${OCLSDKLIB} 
LIBPARS = -lOpenCL -lrt

.PHONY: clean all

all: factor8-1-opencl factor8-1-pas factor8_1.class factor8-1 factor8-1-omp

factor8-1-opencl: factor8-1-opencl.o
	${CPP} ${LFLAGS} -o $@ $< ${LIBPARS}

factor8-1-opencl.o: factor8-1-opencl.cpp
	${CPP} -c ${FLAGS} $<

factor8-1-pas: factor8-1.pas
	fpc -o$@ -O3 -MTP $<
	rm factor8-1.o

factor8_1.class: factor8_1.java
	javac factor8_1.java

factor8-1: factor8-1.o
	${CPP} -o $@ $^

factor8-1.o: factor8-1.c
	${CPP} ${FLAGS} -c -O2 $<

factor8-1-omp: factor8-1-omp.o
	${CPP} -o $@ -fopenmp $^

factor8-1-omp.o: factor8-1.c
	${CPP} ${FLAGS} -c -O2 -o $@ -fopenmp $<

clean:
	rm factor8-1 factor8-1.o factor8-1-omp factor8-1-omp.o factor8-1-pas factor8_1.class factor8-1-opencl factor8-1-opencl.o
