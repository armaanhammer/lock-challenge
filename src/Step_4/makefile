all: lock


lock: lock.o
	g++ -o Lock.out lock.o -pthread

lock.o: main.cpp
	g++ -std=c++11 -c -o lock.o main.cpp


clean:
	rm lock.o Lock.out
