.PHONY: generate-headers build test test1 test2

all: build test

generate-headers: # generates cpp JNI headers for io.jsight.* classes to ./src/cpp folder
	cd docker/generate-headers && docker compose up

build: # builds cpp binary to ./build folder
	docker build -f ./docker/build/Dockerfile --tag jsight:java_lib_builder --progress plain --output build .

test: test1 test2 # runs JUnit tests twice (for two different environments)

test1:
	docker compose -f ./docker/test/docker-compose.yaml up test1

test2:
	docker compose -f ./docker/test/docker-compose.yaml up test2
