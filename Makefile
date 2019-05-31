
all: md5.o carvefile.o fileio.o jpgcarver.exe

md5.o: 
	gcc -c md5.c md5.h

carvefile.o: carvefile.c carvefile.h
	gcc -c $^

fileio.o: fileio.c fileio.h
	gcc -c $^

jpgcarver.exe: md5.o carvefile.o fileio.o jpgcarver.c
	gcc -g -o $@ $^


clean:
	rm -rf *.h.gch jpgcarver.exe *.o *~ *#*
