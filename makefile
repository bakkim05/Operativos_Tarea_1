all: clean client server docker-build docker-run

client: clientside/client.c
	gcc -o clientside/client clientside/client.c

server: serverside/server.c
	gcc -std=c17 -Wall serverside/server.c -o serverside/server -lm

client-run:
	./clientside/client 127.0.0.1

clean:
	rm clientside/client serverside/server

docker-build:
	cd serverside && \
	docker build . -t tarea_1:latest

docker-run:
	cd serverside && \
	docker run -d -p 127.0.0.1:9000:9000 --name instancia -v operativos_tarea_1_volume:/app -v /home/jung/Desktop/REPOS/Operativos_Tarea_1/configuracion/:/app/configuracion tarea_1:latest

docker-stop:
	docker stop instancia
	
docker-clean:
	docker container prune -f