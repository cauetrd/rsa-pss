CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -I. -IMaths
LDFLAGS = -lgmp -lgmpxx

TARGET = run
KEYGEN_TARGET = keygen_exec


SRCS = primeTest.cpp \
       Maths/BigInt/BigInt.cpp \
       Maths/GMP/Miller-Rabin.cpp \
       Maths/GMP/modularArithmetics.cpp \
       Maths/GMP/GCD.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# Compila key_gen (excluindo primeTest.o se já existir)
key_gen: key_gen/key_gen.o $(filter-out primeTest.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $(KEYGEN_TARGET) $^ $(LDFLAGS)

# Compila executável principal
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regra única para compilar qualquer .cpp em qualquer pasta
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza completa
clean:
	find . -name "*.o" -delete
	rm -f $(TARGET) $(KEYGEN_TARGET)

.PHONY: all clean key_gen
