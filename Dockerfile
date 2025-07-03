FROM ubuntu:22.04

RUN apt update && \
    apt install -y g++ make

WORKDIR /app
COPY . .

RUN make

EXPOSE 8080

CMD ["./server"]