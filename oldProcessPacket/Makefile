CXX=g++
CXXFLAGS=-Wall -Wextra 

main: main.o data.o commands.o
	$(CXX) $(CXXFLAGS) main.o data.o commands.o -o main

main.o: main.cpp data.h commands.h
	$(CXX) $(CXXFLAGS) -c main.cpp

data.o: data.cpp data.h
	$(CXX) $(CXXFLAGS) -c data.cpp

commands.o: commands.cpp commands.h
	$(CXX) $(CXXFLAGS) -c commands.cpp

clean:
	-rm -f main *.o
