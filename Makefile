CC = $(shell which gcc || which clang || which cc)
AR = ar
ARFLAGS = rcs
LIBNAME = libglfwcairo.a

UNAME_S := $(shell uname -s || echo Windows_NT)
CC_NAME := $(notdir $(CC))

# Set destination directory including OS and compiler name
DESTDIR := lib/$(UNAME_S)/$(CC_NAME)
CFLAGS := -Wall -Wextra -O3 -std=c99
# Platform-specific flags
ifeq ($(UNAME_S),Linux)
    CFLAGS += -DGLFW_EXPOSE_NATIVE_X11
endif
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -DGLFW_EXPOSE_NATIVE_COCOA
endif
ifeq ($(UNAME_S),Windows_NT)
    CFLAGS += -DGLFW_EXPOSE_NATIVE_WIN32
endif
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: directories $(DESTDIR)/$(LIBNAME)
	rm -rf $(OBJ_DIR)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DESTDIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(DESTDIR)/$(LIBNAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

clean:
	@rm -rf $(OBJ_DIR) $(DESTDIR)/$(LIBNAME)

install: $(DESTDIR)/$(LIBNAME)
    # install -d $(LIBDIR)
    # install -m 644 $(DESTDIR)/$(LIBNAME) $(LIBDIR)
    # install -d $(INCLUDEDIR)
    # install -m 644 include/*.h $(INCLUDEDIR)

.PHONY: all clean install directories
