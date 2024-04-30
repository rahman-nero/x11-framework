PROG = xwindow
SRC = main.c
OBJ = ${SRC:.c=.o}

CC = cc
INCS = -l/usr/include/X11 `pkg-config --cflags xft`
LIBS = -lX11 -lXft

LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra ${INCS}

all: ${PROG}

${PROG}: ${OBJ}
	${CC} -o $@ ${LDFLAGS} ${OBJ}

%.o: %.c
	${CC} -c $< ${CFLAGS}

clean:
	-rm {$OBJ} ${PROG}

.PHONY: all clean