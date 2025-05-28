CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Og -pedantic -fsanitize=undefined
RELEASE_FLAGS = -std=c99 -Wall -Wextra -O2 -DNDEBUG=1
LFLAGS = -I.
TEST_DIR = tests
TARGETS = $(TEST_DIR)/test_range.exe \
		  $(TEST_DIR)/test_stack.exe \
		  $(TEST_DIR)/test_queue.exe \
		  $(TEST_DIR)/test_objpool.exe
HEADERS = range.h stack.h queue.h objpool.h
OBJECTS = $(TARGETS:.exe=.o)

# Default target (debug build)
all: $(TARGETS)

# Build the executable (debug)
%.exe : %.o
	$(CC) $(CFLAGS) -o $@ $<

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

# Build optimized release version
release: clean-objects
	$(MAKE) $(TARGETS) CFLAGS="$(RELEASE_FLAGS)"

# Clean build artifacts
clean:
	rm -f $(TARGETS) $(OBJECTS)

# Clean only object files
clean-objects:
	rm -f $(OBJECTS)

# Rebuild everything
rebuild: clean all

# Mark targets as phony (not file names)
.PHONY: all clean clean-objects rebuild release
