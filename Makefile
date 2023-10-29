SRCS := src/main.c src/pixeltrace.c src/endian.c src/pixeltrace2.c
HDRS := $(patsubst src/%.c,src/%.h,$(SRCS))
OBJS := $(patsubst src/%.c,objs/%.o,$(SRCS))

.SUFFIXES: .c .o

default: FORCE bin/pixeltrace

bin/pixeltrace: $(OBJS) Makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(CFLAGS)

objs/%.o: src/%.c $(HDRS) Makefile
	mkdir -p objs
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

test: FORCE bin/pixeltrace
	pixeltrace testing.bmp > testing.eps
	PIXELTRACE_TYPE=scanline  PIXELTRACE_HEIGHT=0.5                       pixeltrace testing.bmp > testing.2.eps
	PIXELTRACE_TYPE=rectangle PIXELTRACE_HEIGHT=0.5 PIXELTRACE_WIDTH=0.75 pixeltrace testing.bmp > testing.3.eps
	PIXELTRACE_TYPE=circle    PIXELTRACE_HEIGHT=1.0 PIXELTRACE_WIDTH=1.00 pixeltrace testing.bmp > testing.4.eps

.PHONY: FORCE
