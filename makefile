COMPILER=g++-10
LIBS=-lyaml-cpp -lboost_system -lboost_thread -lpthread -lboost_filesystem -lboost_program_options -lfmt
FLAGS=-std=c++17

all: dist git_proxy apt_proxy build

git_proxy: src/proxy/git_proxy.cpp dist
	${COMPILER} -o dist/git_proxy src/proxy/git_proxy.cpp ${LIBS} ${FLAGS}

apt_proxy: src/proxy/apt_proxy.cpp dist
	${COMPILER} -o dist/apt_proxy src/proxy/apt_proxy.cpp ${LIBS} ${FLAGS}

build: src/main.cpp dist
	${COMPILER} -o dist/main src/main.cpp ${LIBS} ${FLAGS}

black_project_init: src/actions/black_project_init.cpp dist
	${COMPILER} -o dist/black_project_init src/actions/black_project_init.cpp ${LIBS} ${FLAGS}

dist:
	mkdir dist

clean:
	rm -rf dist
