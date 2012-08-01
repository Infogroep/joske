PROGRAM=joske

SOURCES=main.c
OBJECTS=$(SOURCES:%.c=%.o)

CWARNINGS=-W -Wall -Wextra -Wundef -Wendif-labels -Wshadow\
					-Wpointer-arith -Wbad-function-cast -Wcast-align\
					-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes\
					-Wnested-externs -Winline -Wdisabled-optimization\
					-Wno-missing-field-initializers
CFLAGS:=-g -O0 -pipe -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L\
				$(CWARNINGS) $(CFLAGS)

CFLAGS+=-I/usr/include/libircclient
LDLIBS+=-lircclient

$(PROGRAM): $(SOURCES)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

test: $(PROGRAM)
	./$<

grindtest: $(PROGRAM)
	valgrind -v --leak-check=full --show-reachable=yes --track-origins=yes ./$<

macrotest: $(SOURCES)
	$(CC) -E $(CFLAGS) $^

clean:
	rm -f $(PROGRAM)

.PHONY: run clean
