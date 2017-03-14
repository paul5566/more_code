SRC = lla.c main.c
DOC = ./result.txt
CFLAGS = -Wall
CFLAGS += -std=c99
CFLAGS += -g

LDFLAGS =

TARGET = lla

all: $(TARGET)
	
lla: $(SRC)
	gcc $(CFLAGS) -o $@ $(SRC)

test:
	touch $(DOC)
	./lla > $(DOC)
clean:
	rm -f $(TARGET) $(DOC)

