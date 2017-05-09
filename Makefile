all: ex

ex: src/thread.c tests/example.c
	gcc src/thread.c tests/example.c -o build/Programme -Wall 

alltests: test1 test11 test12 test2 test21 test22 test23 test31 test32 test51

test1:
	gcc src/thread.c tests/01-main.c -o build/Test1 -Wall -g -O0
test2:
	gcc src/thread.c tests/02-switch.c -o build/Test2 -Wall -g -O0
test11:  
	gcc src/thread.c tests/11-join.c -o build/Test11 -Wall -g -O0
test12:
	gcc src/thread.c tests/12-join-main.c -o build/Test12 -Wall -g -O0
test21:
	gcc src/thread.c tests/21-create-many.c -o build/Test21 -Wall -g -O0
test22:
	gcc src/thread.c tests/22-create-many-recursive.c -o build/Test22 -Wall -g -O0
test23:
	gcc src/thread.c tests/23-create-many-once.c -o build/Test23 -Wall -g -O0
test31:
	gcc src/thread.c tests/31-switch-many.c -o build/Test31 -Wall -g -O0
test32:
	gcc src/thread.c tests/32-switch-many-join.c -o build/Test32 -Wall -g -O0
test51: 
	gcc src/thread.c tests/51-fibonacci.c -o build/Test51 -Wall -g -O0
test61:
	gcc src/thread.c tests/61-mutex.c -o build/Test61 -Wall -g -O0

testpreempt:
	gcc src/thread.c tests/preemption.c -o build/Testpreempt -Wall -g -O0

clean :
	rm -f *.o a.out
	rm -f Programme
	rm -f build/Test*
	rm -f *~ *#
