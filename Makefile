CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I. -IMaths -Ihash -IPSS -IUtils -IRSA -IRSA-PSS -IPEM -Ikey_gen -march=native -DLITTLE_ENDIAN
CFLAGS = -std=c99 -Wall -Wextra -O3 -I. -IMaths -Ihash -march=native -DLITTLE_ENDIAN
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

# Executables - only build what exists
TARGET = run

# Source files
MATH_SRCS = Maths/Miller-Rabin.cpp \
            Maths/modularArithmetics.cpp \
            Maths/GCD.cpp

HASH_SRCS = hash/Keccak-readable-and-compact.c \
            hash/hashFile.cpp

PEM_SRCS = PEM/PEM-read.cpp \
           PEM/PEM-write.cpp

RSA_SRCS = RSA/RSA-encrypt.cpp \
           RSA/RSA-decrypt.cpp

PSS_SRCS = PSS/PSS_encoding.cpp \
           PSS/PSS_verification.cpp \
           PSS/MGF1.cpp

RSA_PSS_SRCS = RSA-PSS/RSAPSS_sign.cpp \
               RSA-PSS/RSAPSS_verify.cpp

UTILS_SRCS = Utils/utils.cpp

COMMON_SRCS = $(MATH_SRCS) $(HASH_SRCS)

TEST_SRCS = test.cpp key_gen/key_gen.cpp $(RSA_PSS_SRCS) $(PSS_SRCS) $(RSA_SRCS) $(HASH_SRCS) $(UTILS_SRCS) $(COMMON_SRCS) $(PEM_SRCS)

define make_objs
$(patsubst %.cpp,%.o,$(filter %.cpp,$1)) \
$(patsubst %.c,%.o,$(filter %.c,$1))
endef

TEST_OBJS = $(call make_objs,$(TEST_SRCS))

ALL_OBJS = $(TEST_OBJS)
DEPS = $(patsubst %.o,%.d,$(ALL_OBJS))

# Default target
all: $(TARGET)

# Linking rules
$(TARGET): $(TEST_OBJS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rules
%.o: %.cpp
	@echo "Compiling C++ $<..."
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

%.o: %.c
	@echo "Compiling C $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

# Test Targets
test: $(TARGET)
	@echo "Running main program..."
	./$(TARGET)

# Build mode shortcuts
debug:
	$(MAKE) BUILD_MODE=debug

release:
	$(MAKE) BUILD_MODE=release

# Cleaning
clean:
	@echo "Cleaning all generated files..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	rm -f $(TARGET)

clean-obj:
	@echo "Cleaning object files and dependencies..."
	find . -name "*.o" -delete
	find . -name "*.d" -delete

distclean: clean

# Help
help:
	@echo "Available targets:"
	@echo "  all             - Build all executables (default)"
	@echo "  $(TARGET)       - Build test program"
	@echo "  $(KEYGEN_TARGET) - Build key generator"
	@echo "  test            - Build and run test program"
	@echo "  test_keygen     - Build and run key generator"
	@echo "  debug           - Build with debug flags"
	@echo "  release         - Build with release flags"
	@echo "  clean           - Remove all generated files"
	@echo "  clean-obj       - Remove only object and dependency files"
	@echo "  help            - Show this help message"

.PHONY: all test debug release clean clean-obj distclean help
