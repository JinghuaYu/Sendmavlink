ifndef VERBOSE
	# Don't show make output about changing directories
	MAKEFLAGS += --no-print-directory
endif

CMAKE_GENERATOR ?= "Unix Makefiles"
MAKE = make
# Use 4 processes in parallel for the usual make
MAKE_ARGS = -j4

# Default is "Release", also possible is "Debug"
BUILD_TYPE ?= "Release"

ROOT_DIR := $(shell pwd)
TOOLCHAIN_ROOT := $(ROOT_DIR)/prebuilts/toolchains
SRC_ROOT := $(ROOT_DIR)/src
BUILD_DIR := $(ROOT_DIR)/build
OUTPUT_DIR := $(ROOT_DIR)/out
OUTPUT_TARGET_DIR := $(OUTPUT_DIR)/target

# Function to create build directory and call make there.
define cmake-build
+@$(eval OUT_DIR = $(OUTPUT_TARGET_DIR)/$(1)/obj)

+@if [ ! -e $(OUT_DIR)/CMakeCache.txt ]; then \
	mkdir -p $(OUT_DIR) \
	&& (cd $(OUT_DIR) \
	&& cmake  $(BUILD_DIR)/$(1) \
	-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	-DSRC_ROOT:STRING=$(SRC_ROOT) \
	-G$(CMAKE_GENERATOR)) \
	|| (rm -rf $(OUTPUT_TARGET_DIR)) \
fi

+@(echo "Build dir: $(OUT_DIR)" \
	&& (cd $(OUT_DIR) && $(MAKE) $(MAKE_ARGS) $(ARGS)) \
  )
endef

all:
	@echo "compile all completely!"
	$(call cmake-build,SendMavlink)

clean:
	@rm -rf $(OUTPUT_DIR)
	@echo "clean all completely!"

.PHONY: clean
