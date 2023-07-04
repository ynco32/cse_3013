cc=gcc #macro => use gcc
cflags = -W -g #compile option: -W: make warning, -g: add debugging info.
objects = main.o count.o digit.o print.o
target = starwars

$(target): $(objects)
	$(cc) $(cflags) -o $(target) $(objects)
#simplify gcc for each objects

%.o: %.c
	$(cc) $(cflags) -c -o $@ $<
#Ever .o file depens on .c files

$(objects) : Header.h
#If Header.h is modified, make new .o fils

.PHONY: clean #use PHONY to prevent Errors
clean:
	rm $(target) $(objects)
#remove target and objects files in this directory
