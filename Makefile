CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I. -IMaths -march=native -DLITTLE_ENDIAN
CFLAGS = -std=c99 -Wall -Wextra -O3 -I. -IMaths -march=native -DLITTLE_ENDIAN
DEBUG_FLAGS = -g -DDEBUG -O0
RELEASE_FLAGS = -DNDEBUG -O3 -march=native
LDFLAGS = -lgmp -lgmpxx

# Build mode: release or debug
BUILD_MODE ?= release
ifeq ($(BUILD_MODE),debug)
    CXXFLAGS += $(DEBUG_FLAGS)
    CFLAGS += $(DEBUG_FLAGS)
else
    CXXFLAGS += $(RELEASE_FLAGS)
    CFLAGS += $(RELEASE_FLAGS)
endif

TARGET = run
KEYGEN_TARGET = keygen_exec
HASHTEST_TARGET = hashtest_exec
SIGN_TARGET = sign_exec
BENCHMARK_TARGET = benchmark_exec
MGF1_TEST_TARGET = mgf1_test_exec


# Source files
COMMON_SRCS = Maths/BigInt/BigInt.cpp \
              Maths/GMP/Miller-Rabin.cpp \
              Maths/GMP/modularArithmetics.cpp \
              Maths/GMP/GCD.cpp \
              hash/Keccak-readable-and-compact.c

PEM_SRCS = PEM/PEM-read.cpp PEM/PEM-write.cpp PEM/PKCS8-PEM.cpp

MAIN_SRCS = primeTest.cpp $(COMMON_SRCS) $(PEM_SRCS)
KEYGEN_SRCS = key_gen.cpp $(COMMON_SRCS) $(PEM_SRCS)
HASHTEST_SRCS = hashTest.cpp hashFile.cpp $(COMMON_SRCS)
SIGN_SRCS = sign.cpp RSA-encrypt.cpp RSA-decrypt.cpp hashFile.cpp $(COMMON_SRCS) $(PEM_SRCS)
BENCHMARK_SRCS = benchmark.cpp $(COMMON_SRCS)
MGF1_TEST_SRCS = PSS/mgf1_test.cpp PSS/MGF1.cpp hash/Keccak-readable-and-compact.c

# Object files (handle both .cpp and .c files)
MAIN_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(MAIN_SRCS))) \
            $(patsubst %.c,%.o,$(filter %.c,$(MAIN_SRCS)))
KEYGEN_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(KEYGEN_SRCS))) \
              $(patsubst %.c,%.o,$(filter %.c,$(KEYGEN_SRCS)))
HASHTEST_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(HASHTEST_SRCS))) \
                $(patsubst %.c,%.o,$(filter %.c,$(HASHTEST_SRCS)))
SIGN_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(SIGN_SRCS))) \
            $(patsubst %.c,%.o,$(filter %.c,$(SIGN_SRCS)))
BENCHMARK_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(BENCHMARK_SRCS))) \
                 $(patsubst %.c,%.o,$(filter %.c,$(BENCHMARK_SRCS)))
MGF1_TEST_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(MGF1_TEST_SRCS))) \
                 $(patsubst %.c,%.o,$(filter %.c,$(MGF1_TEST_SRCS)))
ALL_OBJS = $(sort $(MAIN_OBJS) $(KEYGEN_OBJS) $(HASHTEST_OBJS) $(SIGN_OBJS) $(BENCHMARK_OBJS) $(MGF1_TEST_OBJS))

# Dependency files for automatic header dependency tracking
DEPS = $(patsubst %.o,%.d,$(ALL_OBJS))

# Default target
all: $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET) $(SIGN_TARGET) $(BENCHMARK_TARGET) $(MGF1_TEST_TARGET)

# Main executable
$(TARGET): $(MAIN_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Key generator executable
$(KEYGEN_TARGET): $(KEYGEN_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Hash test executable
$(HASHTEST_TARGET): $(HASHTEST_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Sign executable
$(SIGN_TARGET): $(SIGN_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Benchmark executable
$(BENCHMARK_TARGET): $(BENCHMARK_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# MGF1 test executable
$(MGF1_TEST_TARGET): $(MGF1_TEST_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Convenience targets
key_gen: $(KEYGEN_TARGET)
run_main: $(TARGET)
hash_test: $(HASHTEST_TARGET)
sign: $(SIGN_TARGET)
benchmark: $(BENCHMARK_TARGET)
mgf1_test: $(MGF1_TEST_TARGET)

# Rule to compile .cpp files with automatic dependency generation
%.o: %.cpp
	@echo "Compiling C++ $<..."
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Rule to compile .c files with automatic dependency generation
%.o: %.c
	@echo "Compiling C $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files (ignore if they don't exist)
-include $(DEPS)

# Utility targets
test: $(TARGET)
	@echo "Running prime test program..."
	./$(TARGET)

test_keygen: $(KEYGEN_TARGET)
	@echo "Running key generator..."
	./$(KEYGEN_TARGET)

test_hash: $(HASHTEST_TARGET)
	@echo "Running hash test program..."
	./$(HASHTEST_TARGET)

test_sign: $(SIGN_TARGET)
	@echo "Running sign program..."
	./$(SIGN_TARGET)

test_benchmark: $(BENCHMARK_TARGET)
	@echo "Running benchmark program..."
	./$(BENCHMARK_TARGET)

test_mgf1: $(MGF1_TEST_TARGET)
	@echo "Running MGF1 test program..."
	./$(MGF1_TEST_TARGET)

# Alias for running hash test
hash_test: 
	@echo "Running hash test program..."
	./$(HASHTEST_TARGET)

# Build modes
debug:
	$(MAKE) BUILD_MODE=debug

release:
	$(MAKE) BUILD_MODE=release

# Performance profiling build
profile: CXXFLAGS += -pg -O2
profile: $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET) $(SIGN_TARGET) $(BENCHMARK_TARGET)

# Static analysis
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c++17 --suppress=missingIncludeSystem $(MAIN_SRCS) $(KEYGEN_SRCS) $(HASHTEST_SRCS) $(SIGN_SRCS) $(BENCHMARK_SRCS)

# Clean targets
clean:
	@echo "Cleaning all generated files..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	rm -f $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET) $(SIGN_TARGET) $(BENCHMARK_TARGET)

# Alternative: clean only object files (keep executables)
clean-obj:
	@echo "Cleaning object files and dependencies..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete

distclean: clean

# Install GMP if needed (Ubuntu/Debian)
install-deps:
	@echo "Installing GMP development libraries..."
	sudo apt-get update && sudo apt-get install -y libgmp-dev

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build all executables (default)"
	@echo "  $(TARGET)       - Build prime test executable"
	@echo "  $(KEYGEN_TARGET) - Build key generator"
	@echo "  $(HASHTEST_TARGET) - Build hash test executable"
	@echo "  key_gen      - Alias for $(KEYGEN_TARGET)"
	@echo "  hash_test    - Alias for $(HASHTEST_TARGET)"
	@echo "  test         - Build and run prime test program"
	@echo "  test_keygen  - Build and run key generator"
	@echo "  test_hash    - Build and run hash test program"
	@echo "  test_sign    - Build and run sign program"
	@echo "  test_benchmark - Build and run benchmark program"
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release flags"
	@echo "  profile      - Build with profiling enabled"
	@echo "  analyze      - Run static code analysis"
	@echo "  clean        - Remove all generated files (objects, deps, executables)"
	@echo "  clean-obj    - Remove only object and dependency files"
	@echo "  distclean    - Alias for clean"
	@echo "  install-deps - Install required dependencies"
	@echo "  help         - Show this help message"

.PHONY: all key_gen run_main hash_test sign benchmark test test_keygen test_hash test_sign test_benchmark debug release profile analyze clean clean-obj distclean install-deps help
