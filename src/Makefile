CXX = g++
CXXFLAGS = -std=c++17 -Wall
Llib = ../dep/lib/

all: FITS_MANIP

FITS_MANIP: main.o fits_manipulation.o
	$(CXX) $(CXXFLAGS) main.o fits_manipulation.o -o FITS_MANIP -L$(Llib) -lcfitsio

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

fits_manipulation.o: fits_manipulation.cpp
	$(CXX) $(CXXFLAGS) -c fits_manipulation.cpp

clean:
	rm *.o
