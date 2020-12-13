CC = g++
FLAGS = -std=c++11 -pthread
COMPILE_FLAGS = $(FLAGS) -c
all: PhonePricePrediction.out

PhonePricePrediction.out: main.o
	$(CC) main.o $(FLAGS) -o PhonePricePrediction.out

main.o: main.cpp
	$(CC) $(COMPILE_FLAGS) main.cpp


.PHONY: clean

clean:
				rm *.o
				rm *.out