cc = g++
cflags = -W -g
objects = main.o array.o rangearray.o
target = test

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o: %.cpp
	$(cc) $(cflags) -c -o $@ $<

$(objects) : Array.h RangeArray.h

.PHONY: clean
clean:
	rm $(target) $(objects)
