SRC = get_cost.c main.c linked_list.c
CFLAG = -lm
CFLAG += -lbsd

all: cost

cost: $(SRC)
	gcc -Wall -g -o $@ $(SRC) $(CFLAG)
clean:
	rm -f cost
