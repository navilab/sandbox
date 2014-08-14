# Common makefile definitions.
# @file rsb2_common.mk

#=== Project directories. ===
PROJECT_DIR ?= ../..
SRC_DIR := $(PROJECT_DIR)/src
TESTSUITE_DIR := $(PROJECT_DIR)/testsuite
BUILD_DIR := $(PROJECT_DIR)/build
INSTALL_DIR := $(PROJECT_DIR)/install
EXTINC_DIR := $(PROJECT_DIR)/include
EXTLIB_DIR := $(PROJECT_DIR)/lib
TEST_DIR := $(PROJECT_DIR)/test

LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin
INCLUDE_DIR := $(BUILD_DIR)/include

LD_LIBRARY_PATH := $(LIB_DIR):/usr/local/lib

#=== Build variables. ===
export BUILD_HOST ?= $(shell hostname)
export BUILD_DATE ?= $(shell date +"%Y-%m-%dT%H:%M")
export BUILD_VERSION ?= $(shell hg id -t)
export BUILD_CHECKSUM ?= $(shell hg id -i)

#=== Defines for gcc. ===
DEFINES += -D_GNU_SOURCE
DEFINES += -DRSB2_BUILD_VERSION=\"$(BUILD_VERSION)\"
DEFINES += -DRSB2_BUILD_CHECKSUM=\"$(BUILD_CHECKSUM)\"
DEFINES += -DRSB2_BUILD_HOST=\"$(BUILD_HOST)\"
DEFINES += -DRSB2_BUILD_DATE=\"$(BUILD_DATE)\"

#=== External directories. ===

#=== Files for gcc. ===
SOURCES ?= $(wildcard *.c)
HEADERS ?= $(wildcard *.h)
TEST_SOURCES ?= $(wildcard test/*.c)
INCLUDES += \
	-I $(SRC_DIR)/rsb2/rsb2_libdemo \
	-I $(SRC_DIR)/rsb2/rsb2_libzmq \
	-I $(EXTINC_DIR)
LIBS += -L$(LIB_DIR)

#=== Simulation. ===

#=== Commands and options. ===
MKDIR := mkdir -p
RM := rm -f
CP := cp -p
CFLAGS += -std=c99 -Wall -Wextra -Wshadow -Wno-unused-parameter
ifdef WERROR
CFLAGS += -Werror
endif
ifdef NDEBUG
CFLAGS += -DNDEBUG
endif
CFLAGS += -fPIC -g $(DEFINES)
CC ?= gcc

#=== Paths. ===
vpath %.so $(LIB_DIR)
vpath %.bin $(BIN_DIR)
vpath lib/lib%.so $(BUILD_DIR)
vpath bin/%.bin $(BUILD_DIR)
vpath test/%.bin $(PROJECT_DIR)

#=== Targets. ===
.PHONY: all dist test run clean
all: dist
dist: $(TARGET)
test: test/$(TEST_NAME).bin $(TEST_TARGET)
run: tmp/$(TEST_NAME).run
clean:
	$(RM) $(BUILD_DIR)/$(TARGET)
	$(RM) $(TEST_DIR)/$(TEST_NAME).bin
	$(RM) $(TMP_DIR)/$(TEST_NAME)*

#=== Rule for building a shared library. ===
lib/lib%.so: $(SOURCES) $(HEADERS) $(DEPENDS)
	@test -d $(LIB_DIR) || $(MKDIR) $(LIB_DIR)
	@echo "$@: building shared library..."
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/$@ $(SOURCES) $(INCLUDES) $(LIBS)
	@test -d $(INCLUDE_DIR) || $(MKDIR) $(INCLUDE_DIR)
	@test -z "$(HEADERS)" || cp $(HEADERS) $(INCLUDE_DIR)/

#=== Rule for building a program. ===
bin/%.bin: $(SOURCES) $(HEADERS) $(DEPENDS)
	@test -d $(BIN_DIR) || $(MKDIR) $(BIN_DIR)
	@echo "$@: building program..."
	$(CC) $(CFLAGS) -Wl,-rpath,'$${ORIGIN}'/../lib -o $(DIST_DIR)/$@ $(SOURCES) $(INCLUDES) $(LIBS)

#=== Rule for building a test program. ===
test/%.bin: $(TEST_SOURCES) $(HEADERS) $(DEPENDS)
	@test -d $(TEST_DIR) || $(MKDIR) $(TEST_DIR)
	@echo "$@: building test program..."
	$(CC) $(CFLAGS) -Wl,-rpath,'$${ORIGIN}'/../dist/lib -o $(PROJECT_DIR)/$@ $(TEST_SOURCES) $(INCLUDES) $(LIBS) \
		-L$(LIB_DIR) $(TEST_LIBS)

#=== Rule for running a test program. ===
tmp/%.run: test/%.bin
	@test -d $(TMP_DIR) || $(MKDIR) $(TMP_DIR)
	@echo "$@: running program..."
	cd $(TMP_DIR) && LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) ../test/$*.bin \
		--conf ../src/$(DIR_NAME)/test/$*.conf \
		--ident test_zmq \
		--appid 999 \
		--proc $* \
		--trace debug \
		2>$*.err && touch $*.run

#=== Rule for building an object file. ===
%.o: %.c %.h
	@test -d $(TEST_DIR) || $(MKDIR) $(TEST_DIR)
	@echo "$@: compiling..."
	@$(CC) $(CFLAGS) -c -o $(TEST_DIR)/$@ $< $(INCLUDES)

## END ##
