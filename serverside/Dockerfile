FROM "centos"

EXPOSE 8080 8080

WORKDIR /operativos

COPY . .

RUN yum update -y

RUN yum install gcc -y

CMD ["gcc -std=c17 -Wall server.c -o server -lm"]

CMD ["./server"]


