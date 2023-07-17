export SRC_DIR=/app
export BUILD_DIR=/app/build
export JAVA_HOME=/usr/lib/jvm/default-java

all: compile link

compile:
	mkdir -p ${BUILD_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -I${SRC_DIR} ${SRC_DIR}/io_jsight_JSight.cpp -o ${BUILD_DIR}/io_jsight_JSight.o
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -I${SRC_DIR} ${SRC_DIR}/libjsight_init.c  -o ${BUILD_DIR}/libjsight_init.o

link:
	g++ -shared -fPIC -o ${BUILD_DIR}/libjsightjava.so ${BUILD_DIR}/io_jsight_JSight.o ${BUILD_DIR}/libjsight_init.o -lc

clean:
	rm -rf ${BUILD_DIR} ${JNI_HEADERS_DIR}