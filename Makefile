all: ex

ex: thread.c example.c
	gcc thread.c example.c -o Programme -Wall 

split:
	gcc thread_split.c 

alltests: test1 test11 test12 test2 test21 test22 test23 test31 test32 test51

test1:
	gcc thread.c tests/01-main.c -o Test1 -Wall -g -O0
test2:
	gcc thread.c tests/02-switch.c -o Test2 -Wall -g -O0
test11:  
	gcc thread.c tests/11-join.c -o Test11 -Wall -g -O0
test12:
	gcc thread.c tests/12-join-main.c -o Test12 -Wall -g -O0
test21:
	gcc thread.c tests/21-create-many.c -o Test21 -Wall -g -O0
test22:
	gcc thread.c tests/22-create-many-recursive.c -o Test22 -Wall -g -O0
test23:
	gcc thread.c tests/23-create-many-once.c -o Test23 -Wall -g -O0
test31:
	gcc thread.c tests/31-switch-many.c -o Test31 -Wall -g -O0
test32:
	gcc thread.c tests/32-switch-many-join.c -o Test32 -Wall -g -O0
test51: 
	gcc thread.c tests/51-fibonacci.c -o Test51 -Wall -g -O0
test61:
	gcc thread.c tests/61-mutex.c -o Test61 -Wall -g -O0

clean :
	rm -f *.o a.out
	rm -f Programme
	rm -f Test*
	rm -f *~ *#
