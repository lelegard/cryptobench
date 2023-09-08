#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Makefile for test programs to investigate specifc issues
#----------------------------------------------------------------------------

default: execs

TESTS    = montmult
SHELL    = /usr/bin/env bash --noprofile
SRCDIR   = .
BINDIR   = ../build
EXECS    = $(addprefix $(BINDIR)/,$(TESTS)) $(addprefix $(BINDIR)/,$(addsuffix _static,$(TESTS)))
CFLAGS   += -Werror -Wall -Wextra -Wno-unused-parameter $(if $(DEBUG),-g,-O2)
CPPFLAGS += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include))
LDFLAGS  += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib)) $(if $(DEBUG),-g)
LDLIBS   += -lcrypto

# Build operations.
execs: $(EXECS)
	@true
clean:
	rm -rf ../build ../build-* core ../core *.tmp *.log *.pro.user __pycache__
$(BINDIR)/%: $(SRCDIR)/%.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%_static: $(SRCDIR)/%.o
	$(CC) -static $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# Regenerate implicit dependencies.
ifneq ($(if $(MAKECMDGOALS),$(filter-out clean,$(MAKECMDGOALS)),true),)
    -include $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.d,$(wildcard $(SRCDIR)/*.c))
endif
$(BINDIR)/%.d: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) -MM $(CPPFLAGS) -MT $(BINDIR)/$*.o -MT $@ $< >$@ || rm -f $@
