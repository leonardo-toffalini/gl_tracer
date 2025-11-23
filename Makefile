INCL = /usr/local/include/glad
LINK = $(HOME)/Downloads/thirdparty/glfw-3.4.bin.MACOS/lib-arm64
LIBS = -lglfw3
FRAMEWORKS = \
						 -framework Cocoa \
						 -framework OpenGL \
						 -framework IOKit \
						 -framework CoreVideo \
						 -framework QuartzCore
CFLAGS = -O3
DEBUG_FLAGS = -Wextra -Wall -fsanitize=address
MAIN_FILE = src/main.cpp

.PHONY: run, clean, debug

all: main run

debug: sanitize run

main:
	clang++ -o bin/main $(MAIN_FILE) src/glad.cpp $(CFLAGS) -I$(INCL) -L$(LINK) $(LIBS) $(FRAMEWORKS)

sanitize:
	clang++ -o bin/main $(MAIN_FILE) src/glad.cpp $(DEBUG_FLAGS) -I$(INCL) -L$(LINK) $(LIBS) $(FRAMEWORKS)

run:
	./bin/main

clean:
	rm ./bin/main

