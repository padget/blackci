COMPILER=g++-10
LIBS=-lyaml-cpp -lboost_system -lboost_thread -lpthread -lboost_filesystem -lboost_program_options -lfmt
FLAGS=-std=c++17
EXT=
.PHONY: all

all:\
  dist/blackci \
	dist/proxies_git \
	dist/proxies_apt \
	dist/actions_project \
	dist/actions_project_init \
	dist/filters_paint \
	dist/filters_paint.yml

dist/blackci: src/blackci.cpp dist
	${COMPILER} -o dist/blackci${EXT} src/blackci.cpp ${LIBS} ${FLAGS}

dist/actions_project: src/actions/project.cpp dist
	${COMPILER} -o dist/actions_project${EXT} src/actions/project.cpp ${LIBS} ${FLAGS}

dist/actions_project_init: src/actions/project/init.cpp dist
	${COMPILER} -o dist/actions_project_init${EXT} src/actions/project/init.cpp ${LIBS} ${FLAGS}

dist/filters_paint: src/filters/paint.cpp dist
	${COMPILER} -o dist/filters_paint${EXT} src/filters/paint.cpp ${LIBS} ${FLAGS}
	
dist/proxies_git: src/proxy/git.cpp dist
	${COMPILER} -o dist/proxies_git${EXT} src/proxy/git.cpp ${LIBS} ${FLAGS}

dist/proxies_apt: src/proxy/apt.cpp dist
	${COMPILER} -o dist/proxies_apt${EXT} src/proxy/apt.cpp ${LIBS} ${FLAGS}
	
dist/filters_paint.yml: res/filters_paint.yml
	cp res/filters_paint.yml dist/filters_paint.yml

dist:
	mkdir dist

clean:
	rm -rf dist
