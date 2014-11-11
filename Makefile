all: server client poll
cc=cc

server: server.c src/net/udp/udp_sockets.h src/net/udp/udp_sockets.c src/net/udp/udp_server.h src/net/udp/udp_server.c calculator.h
	$(cc) -o server server.c src/net/udp/udp_sockets.c src/net/udp/udp_server.c

client: client.c src/net/udp/udp_sockets.h src/net/udp/udp_sockets.c src/net/udp/udp_client.h src/net/udp/udp_client.c calculator.h
	$(cc) -o client client.c src/net/udp/udp_sockets.c src/net/udp/udp_client.c

poll: poll.c src/net/udp/udp_sockets.h src/net/udp/udp_sockets.c src/net/udp/udp_server.h src/net/udp/udp_server.c
	$(cc) -o poll poll.c src/net/udp/udp_sockets.c src/net/udp/udp_server.c