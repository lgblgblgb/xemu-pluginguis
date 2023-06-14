SO		= plugingui.so
CC		= g++
LD		= g++
SRC		= haiku.cpp
TEST_PATH	= ~/.local/share/xemu-lgb/mega65/plugingui.so
XEMU_BIN	= ../../xemu-dev/build/bin/xmega65.native
PLUGIN_CFLAGS	= -Wall -O2 -I../common/
COMPILE_CFLAGS	= -fPIC
SDL2_CFLAGS	= $(shell sdl2-config --cflags)
FLTK_CFLAGS	= $(shell fltk-config --cflags)
FLTK_LIBS	= $(shell fltk-config --ldflags)
LDFLAGS		= -shared
OBJ		= $(SRC:.cpp=.o)
DEPENDON	= Makefile ../common/*.h

all:	$(SO)

$(OBJ):	$(SRC) $(DEPENDON)
	$(CC) $(PLUGIN_CFLAGS) $(COMPILE_CFLAGS) $(SDL2_CFLAGS) $(FLTK_CFLAGS) -o $@ -c $<

$(SO):	$(OBJ)
	$(LD) $(LDFLAGS) -o $(SO) $(OBJ) $(FLTK_LIBS)

strip:	$(SO)
	strip $(SO)

install: $(SO)
	$(MAKE) strip
	cp $(SO) $(TEST_PATH)

test:	$(SO)
	cp $(SO) $(TEST_PATH)
	$(XEMU_BIN) -gui plugin | egrep -i 'plugin|gui'

clean:
	rm -f $(SO) $(OBJ)

.PHONY:	all strip install test clean
