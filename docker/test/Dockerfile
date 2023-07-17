FROM maven:latest 
COPY ./jsight-lib/1.0.0/libjsight.so ./build/libjsightjava.so /usr/local/lib/
WORKDIR /app
# fake test run to allow maven to download all its dependencies
RUN mvn test
