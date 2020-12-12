CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: PhonePricePrediction.out

PhonePricePrediction.out: main.o tool.o
	$(CC) main.o tool.o $(FLAGS) -o PhonePricePrediction.out

main.o: main.cpp tool.h
	$(CC) $(COMPILE_FLAGS) main.cpp

tool.o: tool.cpp tool.h
	$(CC) $(COMPILE_FLAGS) tool.cpp

.PHONY: clean

clean:
				rm *.o
				rm *.out
