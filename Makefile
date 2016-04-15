CXX = g++
CXXFLAGS = -lGL -lGLU -lglut

all: assign1

assign1: main.o RgbImage.o
	$(CXX) -o assign1 main.o RgbImage.o $(CXXFLAGS)
	
main.o: main.cpp RgbImage.h
	$(CXX) -c main.cpp  $(CXXFLAGS)

RgbImage.o: RgbImage.cpp RgbImage.h
	$(CXX) -c RgbImage.cpp  $(CXXFLAGS)

clean:
	rm -f *.o assign1
