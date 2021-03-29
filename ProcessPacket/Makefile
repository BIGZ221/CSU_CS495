CXX=g++
CXXFLAGS=-Wall -Wextra 

main: main.o processPacket.o
	$(CXX) $(CXXFLAGS) main.o processPacket.o -o main

main.o: main.cc processPacket.h
	$(CXX) $(CXXFLAGS) -c main.cc

processPacket.o: processPacket.cc processPacket.h
	$(CXX) $(CXXFLAGS) -c processPacket.cc

clean:
	-rm -f main *.o
