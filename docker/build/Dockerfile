FROM debian:buster AS build-stage
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install build-essential -y && \
    apt-get install default-jdk -y
COPY ./src/cpp ./jsight-lib/1.0.0/libjsight.h ./docker/build/Makefile /app/
COPY ./jsight-lib/1.0.0/libjsight.so /usr/local/lib/
WORKDIR /app

RUN make

FROM scratch AS export-stage
COPY --from=build-stage /app/build/libjsightjava.so .