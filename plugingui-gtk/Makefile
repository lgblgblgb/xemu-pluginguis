SO		= plugingui.so
CC		= gcc
LD		= gcc
SRC		= gtk.c
TEST_PATH	= /home/$(shell whoami)/.local/share/xemu-lgb/mega65/plugingui.so
XEMU_BIN	= /home/$(shell whoami)/prog_here/xemu-dev/build/bin/xmega65.native
PLUGIN_CFLAGS	= -Wall -O2 -I../common -fPIC
SDL2_CFLAGS	= $(shell sdl2-config --cflags)
GTK_CFLAGS	= $(shell pkg-config gtk+-3.0 --cflags-only-I)
GTK_LIBS	= $(shell pkg-config gtk+-3.0 --libs)
LDFLAGS		= -shared
OBJ		= $(SRC:.c=.o)

all:	$(SO)

$(OBJ):	$(SRC)
	$(CC) $(PLUGIN_CFLAGS) $(SDL2_CFLAGS) $(GTK_CFLAGS) -o $@ -c $<

$(SO):	$(OBJ)
	$(LD) $(LDFLAGS) -o $(SO) $(OBJ) $(GTK_LIBS)

strip:	$(SO)
	strip $(SO)

install: $(SO)
	$(MAKE) strip
	cp $(SO) $(TEST_PATH)

test:	$(SO)
	cp $(SO) $(TEST_PATH)
	$(XEMU_BIN) -gui plugin | fgrep -i plugin

clean:
	rm -f $(SO) $(OBJ)

.PHONY:	all strip install test clean
