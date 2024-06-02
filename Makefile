PROG = xwindow

PREF_APPLICATION_PAGES = ./application/pages/
PREF_SRC_STORE = ./src/store/
PREF_SRC_ROUTES = ./src/routes/
PREF_SRC = ./src/

SRC = $(wildcard *.c $(PREF_SRC)*.c $(PREF_APPLICATION_PAGES)*.c $(PREF_SRC_STORE)*.c $(PREF_SRC_ROUTES)*.c)
#SRC = $(wildcard *.c)
OBJ = ${SRC:.c=.o}

CC = gcc
INCS = -l/usr/include/X11 `pkg-config --cflags xft` -lc
LIBS = -lX11 -lXft -lpthread -lc

LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra ${INCS}

all: ${PROG} clean

${PROG}: ${OBJ}
	${CC} ${OBJ} -o $@ ${LDFLAGS}

%.o: %.c
	${CC} -c $< -o $@ ${CFLAGS}

clean:
	-rm ${OBJ}

.PHONY: all clean