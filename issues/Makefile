#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Makefile for test programs to investigate specific issues
#----------------------------------------------------------------------------

default: execs

SYSTEM := $(subst Linux,linux,$(subst Darwin,mac,$(shell uname -s)))
ARCH   := $(subst amd64,x64,$(subst x86_64,x64,$(subst aarch64,arm64,$(shell uname -m))))
SHELL   = /usr/bin/env bash --noprofile
SRCDIR  = .
BINDIR  = ../build
TESTS   = montmult $(if $(findstring arm64,$(ARCH)),mult)
EXECS   = $(addprefix $(BINDIR)/,$(TESTS))
STATICS = $(if $(findstring linux,$(SYSTEM)),$(addsuffix _static,$(EXECS)))

# Build with OpenSSL static crypto library if available.
LIBCRYPTO := $(firstword $(wildcard /opt/homebrew/lib/libcrypto.a /usr/local/lib/libcrypto.a /usr/lib*/libcrypto.a /usr/lib*/*/libcrypto.a))

# Build options.
CFLAGS   += -Werror -Wall -Wextra -Wno-unused-parameter $(if $(DEBUG),-g,-O2)
CPPFLAGS += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include))
LDFLAGS  += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib)) $(if $(DEBUG),-g)
LDLIBS   += $(if $(LIBCRYPTO),$(LIBCRYPTO),-lcrypto)

# Build operations.
execs: $(EXECS)
	@true
static: $(STATICS)
	@true
clean:
	rm -rf ../build ../build-* core ../core *.tmp *.log *.pro.user __pycache__
$(BINDIR)/%: $(BINDIR)/%.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%_static: $(BINDIR)/%.o
	$(CC) -static $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
$(BINDIR)/%.o: $(SRCDIR)/%.S
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(BINDIR)/mult_arm.o: CPPFLAGS += $(if $(findstring linux-arm64,$(SYSTEM)-$(ARCH)),-march=armv8.3-a)
$(BINDIR)/mult $(BINDIR)/mult_static: $(if $(findstring arm64,$(ARCH)),$(BINDIR)/mult_arm.o)
$(BINDIR)/montmult $(BINDIR)/montmult_static: $(if $(findstring linux-arm64,$(SYSTEM)-$(ARCH)),$(BINDIR)/montmult_arm1.o $(BINDIR)/montmult_arm2.o)

# Regenerate implicit dependencies.
ifneq ($(if $(MAKECMDGOALS),$(filter-out clean,$(MAKECMDGOALS)),true),)
    -include $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.d,$(wildcard $(SRCDIR)/*.c))
endif
$(BINDIR)/%.d: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) -MM $(CPPFLAGS) -MT $(BINDIR)/$*.o -MT $@ $< >$@ || rm -f $@
