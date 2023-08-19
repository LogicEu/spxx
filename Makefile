# spxx Makefile

SRC=main.c
TARGET=libspxx

CC=gcc
STD=-std=c89
OPT=-O2
WFLAGS=-Wall -Wextra -pedantic
MODULES=spxe spxmath spximg spxplot
OBJDIR=bin
OBJS=$(patsubst %,$(OBJDIR)/%.o,$(MODULES))
LIBS=-lpng -ljpeg -lz -lfreetype -lglfw

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
	DLIB=-dynamiclib
	SUFFIX=dylib
	OSLIBS=-framework OpenGL
else
	DLIB=-shared -fPIC
	SUFFIX=so
	OSLIBS=-lGL -lGLEW -lm
endif

CFLAGS=$(STD) $(OPT) $(WFLAGS)

$(TARGET).$(SUFFIX): $(OBJS)
	$(CC) $(OBJS) -o $@ $(DLIB) $(CFLAGS) $(LIBS) $(OSLIBS)

.PHONY: clean
clean:
	$(shell [ -d $(OBJDIR) ] && rm -r $(OBJDIR))
	$(shell [ -f $(TARGET).$(SUFFIX) ] && rm $(TARGET).$(SUFFIX))

$(OBJDIR)/%.o: % $(OBJDIR) $(SRC)
	$(eval $<_TMP := $(shell echo $< | tr a-z A-Z | sed 's/\(SPX.\)/\-D\1_APPLICATION/g'))
	$(CC) -c $($<_TMP) -include $<.h $(SRC) -o $@ $(CFLAGS) -I$< 

$(OBJDIR):
	$(shell mkdir -p $(OBJDIR))

