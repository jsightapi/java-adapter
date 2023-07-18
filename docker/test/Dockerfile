FROM maven:latest 
WORKDIR /app
# The next lines are the fake test run which allows maven to download all 
# its dependencies in the docker image.
# Real tests are run in docker-compose.yaml file, where all this stuff 
# is binded via volumes.
COPY ./jsight-lib/1.0.0/libjsight.so /usr/local/lib/
COPY ./build/libjsightjava.so /usr/java/packages/lib/
COPY ./pom.xml ./src /app/
RUN mvn test

