OBJS=seri.o

all: obj main #seri.a

CFLAGS=-Iinclude -ggdb3 -Wall -Wno-comment
LDFLAGS=-lrt

OBJS:=$(OBJS:%=obj/%)

obj:
	mkdir -p obj

obj/%.o: text/%.c
	gcc -c -o $@ $< $(CFLAGS) 

#%.a: $(OBJS)
#	gcc $(OBJS) -o $@

main: obj/main.o $(OBJS)
	gcc obj/main.o $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -f main obj/main.o $(OBJS)
