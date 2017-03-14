SRC = get_cost.c main.c linked_list.c
CFLAG = -lm

all: cost

cost: $(SRC)
	gcc -Wall -o $@ $(SRC) $(CFLAG)
clean:
	rm -f cost
