
# ------------------------------------------------
# Makefile for utilities
#
# You can call this Makefile by running
# ```
# make utilities
# ```
# in the root directory
# ------------------------------------------------

ifndef ($(BUILD_DIR))
BUILD_DIR = build
endif

ifndef ($(CORE_DIR))
CORE_DIR = Core
endif

ifndef ($(MIDDLEWARES_DIR))
MIDDLEWARES_DIR = Middlewares
endif

ifndef ($(DRIVERS_DIR))
DRIVERS_DIR = Drivers
endif

ifndef ($(UTILITIES_DIR))
UTILITIES_DIR = Utilities
endif

TARGETS = $(UTILITIES_DIR)/main_clocksync_linux

C_FLAGS = -Wall -pedantic

ifndef ($(C_INCLUDES))
# sets C_INCLUDES
include Includes.inc
endif

all: $(TARGETS)

$(UTILITIES_DIR)/main_clocksync_linux: Utilities/main_clocksync_linux.c Core/Src/main_clocksync.c
	$(CC) $(CFLAGS) $(C_INCLUDES) -ICore/Src $< -o $@
	echo $^
	touch $@

$(BUILD_DIR):
	@mkdir $@

clean:
	-$(RM) $(TARGETS)
