CC = gcc
CFLAGS = -Wall -g -O0
THREAD = src/thread.c

TEST = \
	01-main \
	02-switch \
	11-join \
	12-join-main \
	21-create-many \
	22-create-many-recursive \
	23-create-many-once \
	31-switch-many \
	32-switch-many-join \
	51-fibonacci \

vpath %.c src:tests
vpath % build:build


%: %.c
	$(CC) $(THREAD) -o build/$@ $< $(CFLAGS)

compil-test:$(TEST)

test:compil-test
	./build/01-main 
	./build/02-switch 
	./build/11-join 
	./build/12-join-main 
	./build/21-create-many 2000
	./build/22-create-many-recursive 2000
	./build/23-create-many-once 2000
	./build/31-switch-many 2000 1500
	./build/32-switch-many-join 2000 1500
	./build/51-fibonacci 27


test-valgrind:compil-test
	valgrind ./build/01-main 
	valgrind ./build/02-switch 
	valgrind ./build/11-join 
	valgrind ./build/12-join-main 
	valgrind ./build/21-create-many 2000
	valgrind ./build/22-create-many-recursive 2000
	valgrind ./build/23-create-many-once 2000
	valgrind ./build/31-switch-many 200 150
	valgrind ./build/32-switch-many-join 200 150
	valgrind ./build/51-fibonacci 21

ex: src/thread.c tests/example.c
	gcc src/thread.c tests/example.c -o build/Programme -Wall 

testpreempt:
	gcc src/thread.c tests/preemption.c -o build/Testpreempt -Wall -g -O0

clean:
	rm -f ./build/*
	rm -f *.o a.out
	rm -f Programme
	rm -f *~ *#
