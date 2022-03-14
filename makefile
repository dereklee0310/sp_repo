all: mmap_cp2.c
	gcc mmap_cp2.c -o mmap_cp2
	gcc hole.c -o hole
	gcc mycp2.c -o mycp2
	gcc testFile.c -o test
clean:
	rm -f mmap_cp2 hole mycp2 myHole myHole2 test data1 data2
