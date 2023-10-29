SRCS := src/main.c src/pixeltrace.c src/endian.c
OBJS := objs/main.o objs/pixeltrace.o objs/endian.o
HDRS := src/main.h src/pixeltrace.h src/endian.h
.SUFFIXES: .c .o

default: FORCE bin/pixeltrace

bin/pixeltrace: $(OBJS) Makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(CFLAGS)

objs/%.o: src/%.c $(HDRS) Makefile
	mkdir -p objs
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

test: FORCE bin/pixeltrace
	pixeltrace testing.bmp > testing.eps
	PIXELTRACE_TYPE=scanline PIXELTRACE_HEIGHT=0.5 pixeltrace testing.bmp > testing.2.eps

.PHONY: FORCE
