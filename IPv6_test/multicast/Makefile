SRC_SEND = send.c
SRC_LISTEN = listen.c

all: multi-send multi-listen

multi-send: ${SRC_SEND}
	gcc -Wall -o $@ ${SRC_SEND}
multi-listen: ${SRC_LISTEN}
	gcc -Wall -o $@ ${SRC_LISTEN}
clean:
	rm -f multi-send multi-listen
