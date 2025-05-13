W88_SOURCES=$(wildcard sys/*.c)
W88_OBJECTS=$(addprefix $(BUILDDIR)/,$(notdir $(W88_SOURCES:%.c=%.o)))

WILLOW88=$(BINDIR)/w88

$(WILLOW88): $(W88_OBJECTS) $(BINDIR)
	@echo "  LD	$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(W88_OBJECTS)

$(W88_OBJECTS):build/%.o: sys/%.c $(BUILDDIR)
	@echo "  CC	$@"
	@$(CC) $(CFLAGS) -o $@ -c $<