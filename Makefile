CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -I. -IMaths

TARGET = run

SRCS = primeTest.cpp \
	   BigInt.cpp \
       Maths/Miller-Rabin.cpp Maths/modularArithmetics.cpp Maths/EuclideanExtended.cpp

# Gera lista de objetos: troca .cpp por .o
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# Regra para linkar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra genérica para compilar .cpp em qualquer subpasta
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Funciona mesmo para arquivos como Maths/MillerRabin.cpp -> Maths/MillerRabin.o
Maths/%.o: Maths/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o Maths/*.o $(TARGET)

.PHONY: all clean
