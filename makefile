CC := g++
CFLAGS := -Wall -pthread
TARGET := menu
OBJ := menu.o ImplementacionExacta.o
TARGET_DEBUG := menu-dbg

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/%.o,$(SRCS))
OBJS_DEBUG := $(patsubst src/%.cpp,debug/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -O3

bin/%.o: src/%.cpp
	mkdir --parents bin
	$(CC) $(CFLAGS) -c $< -O3
	mv *.o bin/

clean:
	rm -rf *.o $(TARGET) $(TARGET_DEBUG) bin debug

# changes

debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CC) -o $@ $^ $(CFLAGS) -g -D DEBUG

debug/%.o: src/%.cpp
	mkdir --parents debug
	$(CC) $(CFLAGS) -c $< -g -D DEBUG
	mv *.o debug/