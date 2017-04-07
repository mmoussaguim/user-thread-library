all:
	gcc thread.c example.c -o Programme

thread : thread.c thread.h interface.h
	gcc thread.c 
	
clean :
	rm -f *.o a.out
