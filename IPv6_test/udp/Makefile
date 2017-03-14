SRC_CLI = client.c
SRC_SVR = server.c

all: udp_cli udp_svr

udp_cli: ${SRC_CLI}
	gcc -Wall -o $@ ${SRC_CLI}
udp_svr: ${SRC_SVR}
	gcc -Wall -o $@ ${SRC_SVR}

clean:
	rm -f udp_cli udp_svr
