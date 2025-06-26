CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -I. -IMaths
LDFLAGS = -lgmp -lgmpxx

TARGET = run

SRCS = primeTest.cpp \
       Maths/BigInt/BigInt.cpp \
       Maths/GMP/Miller-Rabin.cpp Maths/GMP/modularArithmetics.cpp Maths/GMP/EuclideanExtended.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Maths/%.o: Maths/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o Maths/GMP/*.o Maths/BigInt/*.o $(TARGET)

.PHONY: all clean
