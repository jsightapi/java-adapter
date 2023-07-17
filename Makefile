.PHONY: generate-headers build test

generate-headers:
	cd docker/generate-headers && docker compose up

build:
	docker build -f ./docker/build/Dockerfile --tag jsight:java_lib_builder --progress plain --output build .

test:
	cd docker/test && docker compose up

