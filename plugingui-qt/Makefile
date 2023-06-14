SO		= plugingui.so
CC		= g++
LD		= g++
SRC		= qt.cpp
TEST_PATH	= /home/$(shell whoami)/.local/share/xemu-lgb/mega65/plugingui.so
XEMU_BIN	= /home/$(shell whoami)/prog_here/xemu-next/build/bin/xmega65.native
PLUGIN_CFLAGS	= -Wall -O3 -I../common/
COMPILE_CFLAGS	= -fPIC
SDL2_CFLAGS	= $(shell sdl2-config --cflags | tr ' ' '\n' | grep '^-I')
#QT_CFLAGS	= -I/usr/include/x86_64-linux-gnu/qt5/ -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets/
#QT_LIBS		= -lQt5Core -lQt5Widgets
QT_CFLAGS	= $(shell pkg-config Qt5Core Qt5Widgets --cflags-only-I)
QT_LIBS		= $(shell pkg-config Qt5Core Qt5Widgets --libs)
LDFLAGS		= -shared
OBJ		= $(SRC:.cpp=.o)
DEPENDON	= Makefile ../common/*.h

all:	$(SO)

$(OBJ):	$(SRC) $(DEPENDON)
	$(CC) $(PLUGIN_CFLAGS) $(COMPILE_CFLAGS) $(SDL2_CFLAGS) $(QT_CFLAGS) -o $@ -c $<

$(SO):	$(OBJ)
	$(LD) $(LDFLAGS) -o $(SO) $(OBJ) $(QT_LIBS)

strip:	$(SO)
	strip $(SO)

install: $(SO)
	$(MAKE) strip
	cp $(SO) $(TEST_PATH)

test:	$(SO)
	cp $(SO) $(TEST_PATH)
	$(XEMU_BIN) -gui plugin | egrep --color -i 'plugin|gui|qt|$(SO)'

clean:
	rm -f $(SO) $(OBJ)

.PHONY:	all strip install test clean
