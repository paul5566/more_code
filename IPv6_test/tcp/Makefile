SRC_CLI = client.c
SRC_SVR = server.c

all: tcp_cli tcp_svr

tcp_cli: ${SRC_CLI}
	gcc -Wall -o $@ ${SRC_CLI}
tcp_svr: ${SRC_SVR}
	gcc -Wall -o $@ ${SRC_SVR}

clean:
	rm -f tcp_cli tcp_svr
