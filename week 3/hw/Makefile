CC = g++
objects = main.o userString.o
target = string

$(target) : $(objects)
	$(CC) -o $(target) $(objects)

$(objects) : userString.h

.PHONY:clean
clean: 
	rm $(objects) $(target)
