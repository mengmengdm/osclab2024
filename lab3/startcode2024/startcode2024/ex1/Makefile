
all : dplist.c dplist.h program.c
	if not exist build mkdir build
	gcc dplist.c program.c -o build/dplistprogram -std=c11 -Wall -g

clean:
	rm -fr *.o build/*
	rm -r build

