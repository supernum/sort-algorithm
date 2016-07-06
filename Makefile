CC = gcc -std=c99 -m64 -g -O0 -Wall 
TESTNAME1 = sort
TESTNAME2 = gen_test_data


INC = -I./
OBJ1 = sort.o
OBJ2 = writedata.o


ALL: $(TESTNAME1) $(TESTNAME2) 
$(TESTNAME1): $(OBJ1) 
	$(CC) -o $@ $^

$(TESTNAME2): $(OBJ2) 
	$(CC) -o $@ $^

%.o : %.c 
	$(CC) -c $< $(INC)


.PHONY:clean
clean:
	rm -f $(OBJ1) $(OBJ2) $(TESTNAME1) $(TESTNAME2)
