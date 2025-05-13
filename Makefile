CFLAGS:=-pedantic -Wall -Wextra -std=c99 -Iinclude
BUILDDIR:=./build
BINDIR:=$(BUILDDIR)/bin

.PHONY: all entry

all: entry

$(BINDIR) $(BUILDDIR):
	@mkdir -p $@

include sys/Makefile
include asm/Makefile

entry: $(WILLOW88) $(WILLOW88-ASM)