BINDIR := bin
SRCDIR := src
OBJDIR := objs

TARGET = $(BINDIR)/pixeltrace

SRCS := $(SRCDIR)/main.c $(SRCDIR)/pixeltrace.c $(SRCDIR)/endian.c $(SRCDIR)/pixeltrace2.c
HDRS := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/%.h,$(SRCS))
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.SUFFIXES: .c .o

default: FORCE $(TARGET)

$(TARGET): $(OBJS) Makefile
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDRS) Makefile
	mkdir -p objs
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

test: FORCE $(TARGET)
	pixeltrace test/testing.bmp > test/testing.eps
	PIXELTRACE_TYPE=scanline  PIXELTRACE_HEIGHT=0.5                       pixeltrace test/testing.bmp > test/testing.2.eps
	PIXELTRACE_TYPE=rectangle PIXELTRACE_HEIGHT=0.5 PIXELTRACE_WIDTH=0.75 pixeltrace test/testing.bmp > test/testing.3.eps
	PIXELTRACE_TYPE=circle    PIXELTRACE_HEIGHT=1.0 PIXELTRACE_WIDTH=1.00 pixeltrace test/testing.bmp > test/testing.4.eps

clean: FORCE
	rm $(OBJS) $(TARGET) 2>/dev/null || true
	find . -type f \( -name '*~' -o -name '#*#' \) -exec rm {} + || true

.PHONY: FORCE
