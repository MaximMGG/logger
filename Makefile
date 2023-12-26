MAIN = logger
LIBS = -l_util

SRC_DIR = src/
OBJ_DIR = obj/
TEST_DIR = test/

SRC = $(wildcard $(SRC_DIR)*.c $(TEST_DIR)*.c)
OBJ = $(patsubst $(SRC_DIR)%.c $(TEST_DIR)%c, $(OBJ_DIR)%.o, $(SRC))


$(MAIN): $(OBJ)
	gcc -o $@ $^ -g $(LIBS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(TEST_DIR)%.c
	gcc -c $^ 

clean:
	rm $(OBJ_DIR)*.c logger
