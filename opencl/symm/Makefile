PATH+=:/usr/local/cuda/bin
CC=g++
TARGETS=symm 
LDLIBS+=-lm -lOpenCL
all: ${TARGETS}
stats:
%: %.cpp
	${CC} ${CFLAGS} ${LDFLAGS} -o $* $< ${LDLIBS}
clean:
	-rm -f ${TARGETS}
