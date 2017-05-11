CC = gcc
CFLAGS = -Wall -g -O0
THREAD = src/thread.c
VLG_OPT = --leak-check=full --track-origins=yes --show-reachable=yes

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
	$(CC) $(THREAD) $(GLOBAL) -o build/$@ $< $(CFLAGS)

compil-test:$(TEST)

test:clean compil-test
	@echo  "\n \033[94m TEST 01-main ...\033[0m"
	@./build/01-main 
	@echo  "\033[94m \nTEST 02-switch ... \033[0m"
	@./build/02-switch 
	@echo  "\n \033[94mTEST 11-join ... \033[0m"
	@./build/11-join 
	@echo  "\n \033[94mTEST 12-join-main ... \033[0m"
	--@./build/12-join-main 
	@echo 	 "\033[94m \nTEST 21-create-many (2000) ... \033[0m"
	@./build/21-create-many 2000
	@echo "\033[94m \nTEST 22-create-many-recurive (2000) ... \033[0m"
	@./build/22-create-many-recursive 2000
	@echo "\033[94m \nTEST 23-create-many-once (2000) ... \033[0m"	
	@./build/23-create-many-once 2000
	@echo "\033[94m \nTEST 31-switch-many (2000 1500) ... \033[0m"
	@./build/31-switch-many 2000 1500
	@echo "\033[94m \nTEST 32-switch-many (2000 1500) ... \033[0m"
	@./build/32-switch-many-join 2000 1500
	@echo "\033[94m \nTEST 51-fibonacci ... \033[0m"
	@./build/51-fibonacci 27


test-valgrind:compil-test
	valgrind $(VLG_OPT) ./build/01-main 
	valgrind $(VLG_OPT) ./build/02-switch 
	valgrind $(VLG_OPT) ./build/11-join 
	valgrind $(VLG_OPT) ./build/12-join-main 
	valgrind $(VLG_OPT) ./build/21-create-many 2000
	valgrind $(VLG_OPT) ./build/22-create-many-recursive 2000
	valgrind $(VLG_OPT) ./build/23-create-many-once 2000
	valgrind $(VLG_OPT) ./build/31-switch-many 200 150
	valgrind $(VLG_OPT) ./build/32-switch-many-join 200 150
	valgrind $(VLG_OPT) ./build/51-fibonacci 21

ex: src/thread.c tests/example.c
	gcc src/thread.c tests/example.c -o build/Programme -Wall 

testpreempt:
	gcc src/thread.c tests/preemption.c -o build/Testpreempt -Wall -g -O0

clean:
	rm -f ./build/*
	rm -f *.o a.out
	rm -f Programme
	rm -f *~ *#
