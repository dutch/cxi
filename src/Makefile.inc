bin_PROGRAMS = %D%/cxi
%C%_cxi_SOURCES = %D%/jsmn.c %D%/jsmn.h %D%/backend.c %D%/backend.h %D%/event.h %D%/cxi.c
if LINUX
%C%_cxi_SOURCES += %D%/event-linux.c
endif
if MAC
%C%_cxi_SOURCES += %D%/event-mac.c
endif
%C%_cxi_CPPFLAGS = $(CURSES_CFLAGS)
%C%_cxi_LDADD = $(CURSES_LIBS)
