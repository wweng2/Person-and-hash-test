CXX = g++
CXXFLAGS = -Wall -g

mytest: hash.o  person.o mytest.cpp
	$(CXX) $(CXXFLAGS) hash.o person.o mytest.cpp -o mytest

hash.o: hash.h hash.cpp person.o
	$(CXX) $(CXXFLAGS) -c hash.cpp

person.o: person.h person.cpp
	$(CXX) $(CXXFLAGS) -c person.cpp


clean:
	rm *.o*
	rm *~


run:
	./mytest


val:
	valgrind ./mytest
