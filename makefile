#!/usr/bin/env make
# shebang allows direct execution with chmod +x (very cursed)

# Specify target when none is provided
.DEFAULT_GOAL := all

# Subdirectories containing course units. Each unit includes its own toolchain
# configuration, so these recursive calls do not need exported variables.
UNITS := 1D_heat_transfer 2D_heat_transfer hello_world tsp_example \
		 histogram add_numbers

# each of these are phony targets, so make will always run the recipe
.PHONY: all clean $(UNITS)

# Run make recursively in each unit's subdirectory.
all: $(UNITS)

# Separate targets for each unit, so they can be built independently.
# Run the 'all' target in a unit's subdirectory.
$(UNITS):
	+$(MAKE) -C $@ all

# Run the 'clean' target in each unit's subdirectory.
# The extra $ is needed to pass arguments to the shell invoked by make.
clean:
	@for unit in $(UNITS); do \
		$(MAKE) -C "$$unit" clean || exit $$?; \
	done
