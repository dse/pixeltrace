BINDIR := bin
SRCDIR := src
OBJDIR := objs

SRCS := $(SRCDIR)/main.c $(SRCDIR)/pixeltrace.c $(SRCDIR)/endian.c $(SRCDIR)/pixeltrace2.c
HDRS := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/%.h,$(SRCS))
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.SUFFIXES: .c .o

default: FORCE $(BINDIR)/pixeltrace

$(BINDIR)/pixeltrace: $(OBJS) Makefile
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDRS) Makefile
	mkdir -p objs
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

test: FORCE $(BINDIR)/pixeltrace
	pixeltrace testing.bmp > testing.eps
	PIXELTRACE_TYPE=scanline  PIXELTRACE_HEIGHT=0.5                       pixeltrace testing.bmp > testing.2.eps
	PIXELTRACE_TYPE=rectangle PIXELTRACE_HEIGHT=0.5 PIXELTRACE_WIDTH=0.75 pixeltrace testing.bmp > testing.3.eps
	PIXELTRACE_TYPE=circle    PIXELTRACE_HEIGHT=1.0 PIXELTRACE_WIDTH=1.00 pixeltrace testing.bmp > testing.4.eps

.PHONY: FORCE
