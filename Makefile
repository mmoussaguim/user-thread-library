all: ex

ex: thread.c example.c
	gcc thread.c example.c -o Programme

split:
	gcc thread_split.c 

test1:
	gcc thread.c tests/01-main.c -o Test1
test2:
	gcc thread.c tests/02-switch.c -o Test2
test11:
	gcc thread.c tests/11-join.c -o Test11
test12:
	gcc thread.c tests/12-join-main.c -o Test12
test21:
	gcc thread.c tests/21-create-many.c -o Test21
test22:
	gcc thread.c tests/02-create-many-recursive.c -o Test22
test23:
	gcc thread.c tests/23-create-many-once.c -o Test23
test31:
	gcc thread.c tests/31-switch-many.c -o Test31
test32:
	gcc thread.c tests/32-switch-many-join.c -o Test32
test51:
	gcc thread.c tests/51-fibonacci.c -o Test51
test61:
	gcc thread.c tests/61-mutex.c -o Test61

clean :
	rm -f *.o a.out
	rm -f Programme
	rm -f Test*
	rm -f *~ *#
