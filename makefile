all: client server

client: clientside/client.c
	gcc -o clientside/client clientside/client.c

server: serverside/server.c
	gcc -std=c17 -Wall serverside/server.c -o serverside/server -lm

run:
	./clientside/client

clean:
	rm clientside/client serverside/server
