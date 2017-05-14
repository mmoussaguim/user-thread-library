CC       = gcc
CFLAGS   = -Wall -g -O0
##THREAD = -DUSE_PTHREAD -lpthread ## OU src/thread.c
THREAD   = src/thread.c
## OU make test THREAD=-DUSE_PTHREAD LIB=-lpthread PATH_BUILD=pthread
VLG_OPT  = --leak-check=full --track-origins=yes --show-reachable=yes
PATH_BUILD = build

TEST     = \
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
	61-mutex \
	preemption \



vpath %.c src:tests
vpath % build:$(PATH_BUILD)


%: %.c
	$(CC) $(GLOBAL) $(THREAD) src/mem_use.c -o $(PATH_BUILD)/$@ $< $(CFLAGS) $(LIB)

compil-test:$(TEST)

test:compil-test
	@echo -e "\n \033[94m TEST 01-main ...\033[0m"
	@./$(PATH_BUILD)/01-main 
	@echo -e "\033[94m \nTEST 02-switch ... \033[0m"
	@./$(PATH_BUILD)/02-switch 
	@echo -e "\n \033[94mTEST 11-join ... \033[0m"
	@./$(PATH_BUILD)/11-join 
	@echo  -e "\n \033[94mTEST 12-join-main ... \033[0m"
	--@./$(PATH_BUILD)/12-join-main 
	@echo -e	 "\033[94m \nTEST 21-create-many (2000) ... \033[0m"
	@./$(PATH_BUILD)/21-create-many 2000
	@echo -e "\033[94m \nTEST 22-create-many-recurive (2000) ... \033[0m"
	@./$(PATH_BUILD)/22-create-many-recursive 500
	@echo -e "\033[94m \nTEST 23-create-many-once (2000) ... \033[0m"	
	@./$(PATH_BUILD)/23-create-many-once 2000
	@echo -e "\033[94m \nTEST 31-switch-many (1000 500) ... \033[0m"
	@./$(PATH_BUILD)/31-switch-many 1000 500
	@echo -e "\033[94m \nTEST 32-switch-many (500 250) ... \033[0m"
	@./$(PATH_BUILD)/32-switch-many-join 500 250
	@echo -e "\033[94m \nTEST 51-fibonacci (27) ... \033[0m"
	@./$(PATH_BUILD)/51-fibonacci 27
	@echo -e "\033[94m \nTEST 61-mutex (42) ... \033[0m"
	@./$(PATH_BUILD)/61-mutex 42


test-valgrind:compil-test
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/01-main 
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/02-switch 
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/11-join 
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/12-join-main 
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/21-create-many 2000
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/22-create-many-recursive 2000
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/23-create-many-once 2000
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/31-switch-many 200 150
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/32-switch-many-join 200 150
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/51-fibonacci 21
	valgrind $(VLG_OPT) ./$(PATH_BUILD)/61-mutex

ex: src/thread.c tests/example.c
	gcc src/thread.c tests/example.c -o $(PATH_BUILD)/Programme -Wall 

testpreempt:
	gcc src/thread.c -DTESTTEMPS tests/preemption.c -o $(PATH_BUILD)/Testpreempt -Wall -g -O0 

clean:
	rm -f ./build/*
	rm -f *.o a.out
	rm -f Programme
	rm -f *~ *#
	rm -f ./pthread/*
