W88A_SOURCES=$(wildcard asm/*.c)
W88A_OBJECTS=$(addprefix $(BUILDDIR)/,$(notdir $(W88A_SOURCES:%.c=%.o)))

WILLOW88-ASM=$(BINDIR)/w88-asm

$(WILLOW88-ASM): $(W88A_OBJECTS) $(BINDIR)
	@echo "  LD	$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(W88A_OBJECTS)

$(W88A_OBJECTS):build/%.o: asm/%.c $(BUILDDIR)
	@echo "  CC	$@"
	@$(CC) $(CFLAGS) -o $@ -c $<