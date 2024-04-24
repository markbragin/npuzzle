CC := gcc
CFLAGS := -Wall -Wpedantic -g -ansi -pedantic
LDFLAGS := -lm

BINARY := npuzzle
BUILD_DIR := build
SOURCE_DIR := src

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

INCLUDE_DIRS := $(shell find $(SRC_DIRS) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INC_DIRS))

DEPS := $(OBJECTS:%.o=%.d)

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE_FLAGS) $< -MM -MT $(@:%.d=%.o) > $@

%.o:
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean run release

clean:
	rm -rf $(OBJECTS) $(BINARY)

run: all
	@./$(BINARY)

release: CFLAGS := -s -O2
release: all

-include $(DEPS)
