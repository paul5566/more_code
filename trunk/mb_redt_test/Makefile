GUEST_SRC = guest.c 
HOST_SRC = host.c advLinkedList.c
LDFLAGS = -pthread
CFLAGS = -Wall
CFLAGS += -std=c99
CFLAGS += -D_GNU_SOURCE
CFLAGS += -g

TARGET = host
TARGET += guest

all: $(TARGET)

host: $(HOST_SRC)
	gcc $(CFLAGS) -o $@ $(HOST_SRC) $(LDFLAGS)

guest: $(GUEST_SRC)
	gcc $(CFLAGS) -o $@ $(GUEST_SRC) $(LDFLAGS) -lbsd

clean:
	rm -f $(TARGET)
