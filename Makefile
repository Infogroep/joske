PROGRAM=joske

SOURCES=main.c
OBJECTS=$(SOURCES:%.c=%.o)

CWARNINGS=-W -Wall -Wextra -Wundef -Wendif-labels -Wshadow\
					-Wpointer-arith -Wbad-function-cast -Wcast-align\
					-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes\
					-Wnested-externs -Winline -Wdisabled-optimization\
					-Wno-missing-field-initializers -Wno-unused
CFLAGS:=-g -O0 -pipe -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L\
				$(CWARNINGS) $(CFLAGS)

# TODO clean up libircclient hackery :)
LIBIRCCLIENT_LOCAL=libircclient/src/libircclient.o

CFLAGS+=-Ilibircclient/include
LDLIBS+=-Llibircclient/src -lircclient

$(PROGRAM): $(SOURCES) | $(LIBIRCCLIENT_LOCAL)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(LIBIRCCLIENT_LOCAL):
	./getlibircclient.sh

test: $(PROGRAM)
	./$<

grindtest: $(PROGRAM)
	valgrind -v --leak-check=full --show-reachable=yes --track-origins=yes ./$<

macrotest: $(SOURCES)
	$(CC) -E $(CFLAGS) $^

clean:
	rm -f $(PROGRAM)

veryclean: clean
	rm -rf libircclient/

.PHONY: run clean veryclean
