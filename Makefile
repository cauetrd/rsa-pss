CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I. -IMaths -march=native
CFLAGS = -std=c99 -Wall -Wextra -O3 -I. -IMaths -march=native
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


# Source files
COMMON_SRCS = Maths/BigInt/BigInt.cpp \
              Maths/GMP/Miller-Rabin.cpp \
              Maths/GMP/modularArithmetics.cpp \
              Maths/GMP/GCD.cpp \
              Keccak-readable-and-compact.c

MAIN_SRCS = primeTest.cpp $(COMMON_SRCS)
KEYGEN_SRCS = key_gen/key_gen.cpp $(COMMON_SRCS)
HASHTEST_SRCS = hashTest.cpp $(COMMON_SRCS)

# Object files (handle both .cpp and .c files)
MAIN_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(MAIN_SRCS))) \
            $(patsubst %.c,%.o,$(filter %.c,$(MAIN_SRCS)))
KEYGEN_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(KEYGEN_SRCS))) \
              $(patsubst %.c,%.o,$(filter %.c,$(KEYGEN_SRCS)))
HASHTEST_OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(HASHTEST_SRCS))) \
                $(patsubst %.c,%.o,$(filter %.c,$(HASHTEST_SRCS)))
ALL_OBJS = $(sort $(MAIN_OBJS) $(KEYGEN_OBJS) $(HASHTEST_OBJS))

# Dependency files for automatic header dependency tracking
DEPS = $(patsubst %.o,%.d,$(ALL_OBJS))

# Default target
all: $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET)

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

# Convenience targets
key_gen: $(KEYGEN_TARGET)
run_main: $(TARGET)
hash_test: $(HASHTEST_TARGET)

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
profile: $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET)

# Static analysis
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c++17 --suppress=missingIncludeSystem $(MAIN_SRCS) $(KEYGEN_SRCS) $(HASHTEST_SRCS)

# Clean targets
clean:
	@echo "Cleaning all generated files..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	rm -f $(TARGET) $(KEYGEN_TARGET) $(HASHTEST_TARGET)

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
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release flags"
	@echo "  profile      - Build with profiling enabled"
	@echo "  analyze      - Run static code analysis"
	@echo "  clean        - Remove all generated files (objects, deps, executables)"
	@echo "  clean-obj    - Remove only object and dependency files"
	@echo "  distclean    - Alias for clean"
	@echo "  install-deps - Install required dependencies"
	@echo "  help         - Show this help message"

.PHONY: all key_gen run_main hash_test test test_keygen test_hash debug release profile analyze clean clean-obj distclean install-deps help
