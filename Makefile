

LD := cc
CC := cc

INCLUDE_FLAGS := -Isrc
CFLAGS := $(INCLUDE_FLAGS) -g -O0 -std=c11 -Wall -Wextra -Wpedantic

EXEC_NAME := a.out
EXEC_SOURCES := src/darr.c src/mathd.c src/main.c
EXEC_OBJECTS := build/darr.c.o build/mathd.c.o build/main.c.o

all: build

build: $(EXEC_NAME)

clean:
	rm -rf build
	rm -f $(EXEC_NAME)

run:
	./$(EXEC_NAME)


$(EXEC_NAME): build/$(EXEC_NAME)
	ln -f build/$(EXEC_NAME) $(EXEC_NAME)
	chmod +x $(EXEC_NAME)

build/$(EXEC_NAME): $(EXEC_OBJECTS)
	mkdir -p build
	$(LD) -o $@ $(EXEC_OBJECTS) $(LFLAGS)

build/%.c.o: src/%.c
	mkdir -p build
	$(CC) -c -o $@ $< $(CFLAGS)

