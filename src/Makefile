.SUFFIXES: .lo

DEFAULT_ZONEINFO=/usr/share/zoneinfo
PREFIX=/usr/local
INCLUDEDIR=$(PREFIX)/include
LIBDIR=$(PREFIX)/lib

CC=gcc
LD=gcc
AR=ar
INSTALL=install
LN_S=ln -s

CFLAGS=-O5 -Wall -Werror
SHCFLAGS=-fPIC
CPPFLAGS=-I../include -DDEFAULT_ZONEINFO=\"$(DEFAULT_ZONEINFO)\"
SHCPPFLAGS=$(CPPFLAGS)
SHLD=$(LD) -shared
LIBEXT=so
OS=$(shell uname)

TZ_MAJOR=1
TZ_MINOR=0
TZ_MICRO=0

OBJS=tzinfo.o
OBJS_LO=$(OBJS:%.o=%.lo)

SHLDFLAGS+=$(VENDOR_LDFLAGS) -m64 -L$(LIBDIR)
ifeq ($(OS),Darwin)
SHLDFLAGS+=-current_version $(TZ_MAJOR).$(TZ_MINOR).$(TZ_MICRO) -install_name $(LIBDIR)/libtz.$(TZ_MAJOR).dylib
SOLONG=libtz.$(TZ_MAJOR).$(TZ_MINOR).$(TZ_MICRO).dylib
SOSHORT=libtz.$(TZ_MAJOR).dylib
LIBEXT=dylib
else
SHLDFLAGS+=-Wl,-soname,libtz.so.$(TZ_MAJOR)
SOLONG=libtz.so.$(TZ_MAJOR).$(TZ_MINOR).$(TZ_MICRO)
SOSHORT=libtz.so.$(TZ_MAJOR)
endif
LIBNAME=libtz.$(LIBEXT)

all:	libtz.a libtz.$(LIBEXT) test

libtz.a:	$(OBJS)
	$(AR) crf $@ $(OBJS)

libtz.$(LIBEXT):	$(OBJS_LO)
	$(SHLD) $(EXTRA_SHLDFLAGS) $(SHLDFLAGS) -o $@ $(OBJS_LO)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.c.lo:
	$(CC) $(SHCPPFLAGS) $(SHCFLAGS) $(CFLAGS) -o $@ -c $<

test:	test.o libtz.a
	$(CC) -o $@ test.o libtz.a

check:	test
	./test

clean:
	rm -f *.o *.lo libtz.a libtz.so test

install:        all
	$(INSTALL) -d $(DESTDIR)/$(INCLUDEDIR)
	$(INSTALL) -m 0444 ../include/libtz.h $(DESTDIR)/$(INCLUDEDIR)/libtz.h
	$(INSTALL) -d $(DESTDIR)/$(LIBDIR)
	$(INSTALL) -m 0444 libtz.a $(DESTDIR)/$(LIBDIR)/libtz.a
	$(INSTALL) -m 0555 libtz.$(LIBEXT) $(DESTDIR)/$(LIBDIR)/$(SOLONG)
	$(LN_S) -f $(SOLONG) $(DESTDIR)/$(LIBDIR)/$(SOSHORT)
	$(LN_S) -f $(SOLONG) $(DESTDIR)/$(LIBDIR)/$(LIBNAME)

tzinfo.c:	../include/libtz.h
