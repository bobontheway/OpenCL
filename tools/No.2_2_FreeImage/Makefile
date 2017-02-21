all:
	g++ -Wall -c -o util.o ../lib/util.c
	g++ -Wall -I ../include -c -o FreeImage.o FreeImage.c
	g++ -lfreeimage -o FreeImage util.o FreeImage.o

clean:
	rm *.o *.bin *.png FreeImage
