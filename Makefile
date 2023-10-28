SRCS := src/main.c src/pixeltrace.c src/endian.c
OBJS := objs/main.o objs/pixeltrace.o objs/endian.o
HDRS := src/main.h src/pixeltrace.h src/endian.h
.SUFFIXES: .c .o

default: FORCE bin/pixeltrace2

bin/pixeltrace2: $(OBJS) Makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(CFLAGS)

objs/%.o: src/%.c $(HDRS) Makefile
	mkdir -p objs
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

.PHONY: FORCE
