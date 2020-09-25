FROM centos:latest as build-env
RUN yum -y update && \
    yum -y install gcc

WORKDIR /app

COPY . .

RUN gcc -o server server.c -lm


FROM centos:latest

COPY --from=build-env /app/server /app/server

WORKDIR /app

CMD ["/app/server"]
