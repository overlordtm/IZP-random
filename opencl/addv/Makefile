CC=gcc
TARGETS=addv 
LDLIBS+=-lm -lOpenCL
all: ${TARGETS}
stats:
%: %.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $* $< ${LDLIBS}
clean:
	-rm -f ${TARGETS}
