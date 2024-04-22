CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -g -ansi -pedantic
LDFLAGS :=

BINARY := npuzzle
BUILD_DIR := build
SOURCE_DIR := src
TESTS_DIR := tests

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

INCLUDE_DIRS := $(shell find $(SRC_DIRS) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INC_DIRS))

TESTS_SOURCES := $(shell find $(TESTS_DIR) -name '*.c')
TESTS_OBJECTS := $(TESTS_SOURCES:%.c=$(BUILD_DIR)/%.o)
TESTS_OBJECTS += $(TESTS_OBJECTS:$(BUILD_DIR)/$(TESTS_DIR)/%_test.o=$(BUILD_DIR)/%.o)
TESTS_BINARY := run_tests

DEPS := $(OBJECTS:%.o=%.d) $(TESTS_OBJECTS:%.o=%.d)

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE_FLAGS) $< -MM -MT $(@:%.d=%.o) > $@

$(BUILD_DIR)/$(TESTS_DIR)/%.d: $(TESTS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE_FLAGS) $< -MM -MT $(@:%.d=%.o) > $@

%.o:
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean run check test

clean:
	rm -rf $(OBJECTS) $(TESTS_OBJECTS) $(BINARY) $(TESTS_BINARY)

run: all
	@./$(BINARY)

check test: $(TESTS_BINARY)
	@./$(TESTS_BINARY)

$(TESTS_BINARY): $(TESTS_OBJECTS)
	$(CC) $(CFLAGS) -o $(TESTS_BINARY) $^ $(LDFLAGS)

-include $(DEPS)
