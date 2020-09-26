FROM centos:latest as build-env

RUN yum -y update && \
    yum -y install gcc

WORKDIR /app

COPY . .

RUN gcc -o server server.c -lm

FROM centos:latest

COPY --from=build-env /app/server /app/server

RUN mkdir app/configuracion

RUN mkdir app/red

RUN mkdir app/green

RUN mkdir app/blue

RUN mkdir app/Not_Trusted

WORKDIR /app

EXPOSE 9000

CMD ["/app/server"]

