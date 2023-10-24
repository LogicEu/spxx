# spxx Makefile

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

.PHONY: clean static shared all

all: static shared

static: $(TARGET).a

shared: $(TARGET).$(SUFFIX)

$(TARGET).a: $(OBJS)
	ar -cr $@ $^

$(TARGET).$(SUFFIX): $(OBJS)
	$(CC) $(OBJS) -o $@ $(DLIB) $(CFLAGS) $(LIBS) $(OSLIBS)

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(TARGET).$(SUFFIX)
	$(RM) $(TARGET).a

$(OBJDIR)/%.o: % $(OBJDIR)
	$(eval $<_TMP := $(shell echo $< | tr a-z A-Z | sed 's/\(SPX.\).*/\-D\1_APPLICATION /g'))
	$(CC) -x c $</$<.h $($<_TMP) -c -o $@ $(CFLAGS) -I$<

$(OBJDIR):
	$(shell mkdir -p $(OBJDIR))

