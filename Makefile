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
	@echo -e "\033[94m TEST 01-main ...\033[0m"
	@./build/01-main 
	@echo -e "\033[94m \nTEST 02-switch ... \033[0m"
	@./build/02-switch 
	@echo -e "\n \033[94mTEST 11-join ... \033[0m"
	@./build/11-join 
	@echo -e "\n \033[94mTEST 12-join-main ... \033[0m"
	@./build/12-join-main 
	@echo -e "\033[94m \nTEST 21-create-many (2000) ... \033[0m"
	@./build/21-create-many 2000
	@echo -e "\033[94m \nTEST 22-create-many-recurive (2000) ... \033[0m"
	@./build/22-create-many-recursive 2000
	@echo -e "\033[94m \nTEST 23-create-many-once (2000) ... \033[0m"	
	@./build/23-create-many-once 2000
	@echo -e "\033[94m \nTEST 31-switch-many (2000 1500) ... \033[0m"
	@./build/31-switch-many 2000 1500
	@echo -e "\033[94m \nTEST 32-switch-many (2000 1500) ... \033[0m"
	@./build/32-switch-many-join 2000 1500
	@echo -e "\033[94m \nTEST 51-fibonacci ... \033[0m"
	@./build/51-fibonacci 27


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
