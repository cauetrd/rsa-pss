CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I. -IMaths -march=native
DEBUG_FLAGS = -g -DDEBUG -O0
RELEASE_FLAGS = -DNDEBUG -O3 -march=native
LDFLAGS = -lgmp -lgmpxx

# Build mode: release or debug
BUILD_MODE ?= release
ifeq ($(BUILD_MODE),debug)
    CXXFLAGS += $(DEBUG_FLAGS)
else
    CXXFLAGS += $(RELEASE_FLAGS)
endif

TARGET = run
KEYGEN_TARGET = keygen_exec


# Source files
COMMON_SRCS = Maths/BigInt/BigInt.cpp \
              Maths/GMP/Miller-Rabin.cpp \
              Maths/GMP/modularArithmetics.cpp \
              Maths/GMP/GCD.cpp

MAIN_SRCS = primeTest.cpp $(COMMON_SRCS)
KEYGEN_SRCS = key_gen/key_gen.cpp $(COMMON_SRCS)

# Object files
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
KEYGEN_OBJS = $(KEYGEN_SRCS:.cpp=.o)
ALL_OBJS = $(sort $(MAIN_OBJS) $(KEYGEN_OBJS))

# Dependency files for automatic header dependency tracking
DEPS = $(ALL_OBJS:.o=.d)

# Default target
all: $(TARGET) $(KEYGEN_TARGET)

# Main executable
$(TARGET): $(MAIN_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Key generator executable
$(KEYGEN_TARGET): $(KEYGEN_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Convenience targets
key_gen: $(KEYGEN_TARGET)
run_main: $(TARGET)

# Rule to compile .cpp files with automatic dependency generation
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files (ignore if they don't exist)
-include $(DEPS)

# Utility targets
test: $(TARGET)
	@echo "Running main program..."
	./$(TARGET)

test_keygen: $(KEYGEN_TARGET)
	@echo "Running key generator..."
	./$(KEYGEN_TARGET)

# Build modes
debug:
	$(MAKE) BUILD_MODE=debug

release:
	$(MAKE) BUILD_MODE=release

# Performance profiling build
profile: CXXFLAGS += -pg -O2
profile: $(TARGET) $(KEYGEN_TARGET)

# Static analysis
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c++17 --suppress=missingIncludeSystem $(MAIN_SRCS) $(KEYGEN_SRCS)

# Clean targets
clean:
	@echo "Cleaning all generated files..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	rm -f $(TARGET) $(KEYGEN_TARGET)

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
	@echo "  all          - Build both executables (default)"
	@echo "  $(TARGET)       - Build main executable"
	@echo "  $(KEYGEN_TARGET) - Build key generator"
	@echo "  key_gen      - Alias for $(KEYGEN_TARGET)"
	@echo "  test         - Build and run main program"
	@echo "  test_keygen  - Build and run key generator"
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release flags"
	@echo "  profile      - Build with profiling enabled"
	@echo "  analyze      - Run static code analysis"
	@echo "  clean        - Remove all generated files (objects, deps, executables)"
	@echo "  clean-obj    - Remove only object and dependency files"
	@echo "  distclean    - Alias for clean"
	@echo "  install-deps - Install required dependencies"
	@echo "  help         - Show this help message"

.PHONY: all key_gen run_main test test_keygen debug release profile analyze clean clean-obj distclean install-deps help
