CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Og -pedantic -fsanitize=undefined
RELEASE_FLAGS = -std=c99 -Wall -Wextra -O2 -DNDEBUG=1
LFLAGS = -I.
TEST_DIR = tests
TARGET = $(TEST_DIR)/test_range
SOURCES = $(TEST_DIR)/test_range.c
OBJECTS = $(SOURCES:.c=.o)

# Default target (debug build)
all: $(TARGET)

# Build the executable (debug)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

# Build optimized release version
release: clean-objects
	$(MAKE) $(TARGET) CFLAGS="$(RELEASE_FLAGS)"

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJECTS)

# Clean only object files
clean-objects:
	rm -f $(OBJECTS)

# Rebuild everything
rebuild: clean all

# Mark targets as phony (not file names)
.PHONY: all clean clean-objects rebuild release
