CXX = g++
CXXFLAGS = -lGL -lGLU -lglut

all: assign1

assign1: main.o RgbImage.o
	$(CXX) $(CXXFLAGS) -o assign1 main.o RgbImage.o
	
main.o: main.cpp RgbImage.h
	$(CXX) $(CXXFLAGS) -c main.cpp

RgbImage.o: RgbImage.cpp RgbImage.h
	$(CXX) $(CXXFLAGS) -c RgbImage.cpp

clean:
	rm -f *.o assign1
