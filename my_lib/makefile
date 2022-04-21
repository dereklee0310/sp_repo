all: lib_driver

lib_driver: lib_driver.o my_lib.o
	gcc lib_driver.o my_lib.o -o lib_driver
	
lib_driver.o: lib_driver.c	
	gcc lib_driver.c -c

my_lib.o: my_lib.c my_lib.h
	gcc -c my_lib.c

clean:
	rm -rf *.o lib_driver input output