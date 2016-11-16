#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -std=gnu99 -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
LDLIBS += -lm
CPPFLAGS +=
LDFLAGS +=


all: game

game: framebuffer_painter.o graphic_engine.o breakout.o

clean:
	-rm -f game

install:


.PHONY: all install clean

# end of development makefile
