PROG = xwindow

PREF_SRC_CONFIG = ./src/config/
PREF_SRC_PAGES = ./src/pages/
PREF_SRC_UTILS = ./src/utils/
PREF_SRC_STORE = ./src/store/

SRC = $(wildcard $(PREF_SRC_PAGES)*.c *.c $(PREF_SRC_CONFIG)*.c $(PREF_SRC_UTILS)*.c $(PREF_SRC_STORE)*.c)
#SRC = $(wildcard *.c)
OBJ = ${SRC:.c=.o}

CC = gcc
INCS = -l/usr/include/X11 `pkg-config --cflags xft`
LIBS = -lX11 -lXft -lpthread

LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra ${INCS}

all: ${PROG}

${PROG}: ${OBJ}
	${CC} ${OBJ} -o $@ ${LDFLAGS}

%.o: %.c
	${CC} -c $< -o $@ ${CFLAGS}

clean:
	-rm ${OBJ} ${PROG}

.PHONY: all clean