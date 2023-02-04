CC = clang++
CFLAGS = -Wall -Wextra

all: main.cpp
	$(CC) main.cpp -o main.exe $(CFLAGS)

run: main.exe
	./main.exe
