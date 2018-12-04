all: lock


hello: hello.o
	g++ -o Hello.out hello.o

hello.o: main.cpp
	g++ -std=c++11 -c -o hello.o hello.cpp


lock: lock.o
	g++ -o Lock.out lock.o -pthread

lock.o: main.cpp
	g++ -std=c++11 -c -o lock.o main.cpp


clean:
	rm hello.o Hello.out lock.o Lock.out
