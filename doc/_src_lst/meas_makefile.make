SHELL = /bin/sh
CC    = gcc
 
FLAGS        = -std=gnu99 
CFLAGS       = -Wall -Wextra -march=native -ggdb3
LIBS         = -lbsd
DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -fwhole-program
 
TARGET  = meas
SOURCES = $(shell echo src/*.c)
COMMON  = 
HEADERS = $(shell echo src/*.h)
OBJECTS = $(SOURCES:.c=.o)

PREFIX  = $(DESTDIR)/usr/local
BINDIR  = $(PREFIX)/bin
SRCDIR  = src

all: $(TARGET)
	 
$(TARGET): $(OBJECTS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)

release: $(OBJECTS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(RELEASEFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)

profile: CFLAGS += -pg
profile: $(TARGET)

install: release
	install -D $(TARGET) $(BINDIR)/$(TARGET)
		 
install-strip: release
	install -D -s $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	-rm $(BINDIR)/$(TARGET)
		  
clean:
	-rm -f $(OBJECTS)
	-rm -f gmon.out
	-rm -f $(TARGET)

distclean: clean
	-rm -f $(BINDIR)/$(TARGET)

.PHONY : all profile release \
	install install-strip uninstall clean distclean

$(SRCDIR)/*.o: .FORCE

.FORCE:
