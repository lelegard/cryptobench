#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Makefile for the cryptobench tool.
#----------------------------------------------------------------------------

default all: exec

# SYSTEM = linux or mac, ARCH = x64 or arm64
SYSTEM := $(subst Linux,linux,$(subst Darwin,mac,$(shell uname -s)))
ARCH   := $(subst amd64,x64,$(subst x86_64,x64,$(subst aarch64,arm64,$(shell uname -m))))

# Project directories and files.
SRCDIR   = .
BINDIR   = ../build
EXEC     = $(BINDIR)/cryptobench
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

# Tools and general options.
SHELL      = /usr/bin/env bash --noprofile
CXXFLAGS  += -std=c++17 -Werror -Wall -Wextra -Wno-unused-parameter
FULLSPEED  = -O3 -fno-strict-aliasing -funroll-loops -fomit-frame-pointer
CPPFLAGS  += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include))
LDFLAGS   += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib))
LDLIBS    += -ltomcrypt -ltommath -lgnutls -lhogweed -lnettle -lmbedcrypto -lcrypto -lgmp -lm
NM         = nm
NMFLAGS    = $(if $(findstring linux,$(SYSTEM)),-D)

# Old gcc versions.
LDLIBS += $(if $(wildcard /usr/lib/gcc/*/*/libstdc++fs.*),-lstdc++fs)

# Define DEBUG to compile in debug mode.
CXXFLAGS += $(if $(DEBUG),-g,-O2)
LDFLAGS  += $(if $(DEBUG),-g)

# Define CFI to enable Control Flow Integrity features.
ifneq ($(CFI),)
    ifeq ($(SYSTEM),mac)
        # See https://github.com/lelegard/arm-cpusysregs/blob/main/docs/arm64e-on-macos.md
        CFIFLAGS := $(if $(shell (uname -m | grep arm64e) || ((csrutil status | grep disabled) && (nvram -p | grep '^boot-args.*-arm64e_preview_abi'))),-arch arm64e)
        CXXFLAGS += $(CFIFLAGS)
        LDFLAGS  += $(CFIFLAGS)
    else
        CXXFLAGS += -mbranch-protection=standard
    endif
endif

# Check if LibTomCrypt is built with support for LibTomMath and GMP.
# On most distros, it is built with both. But there are exceptions.
# Example: On Fedora and Red Hat, LibTomCrypt does not support GMP.
LIBTOM     = $(wildcard /usr/lib*/libtomcrypt.so /usr/lib*/*/libtomcrypt.so /opt/homebrew/lib/libtomcrypt.dylib /usr/local/lib/libtomcrypt.dylib)
LIBTOMSYMS = $(shell $(NM) $(NMFLAGS) $(LIBTOM) 2>/dev/null | grep -e ltm_desc -e gmp_desc)
CPPFLAGS  += $(if $(findstring ltm_desc,$(LIBTOMSYMS)),-DLTM_DESC)
CPPFLAGS  += $(if $(findstring gmp_desc,$(LIBTOMSYMS)),-DGMP_DESC)

# On Arm64 systems, compile the accelerated modules with the right target arhitecture level.
# No need to specify this on macOS, this is the default on Arm64.
$(BINDIR)/arm64.o: CXXFLAGS += $(FULLSPEED) $(if $(findstring linux-arm64,$(SYSTEM)-$(ARCH)),-march=armv8-a+crypto)

# Build operations.
exec: $(EXEC)
	@true
$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
clean:
	rm -rf ../build ../build-* core ../core *.tmp *.log *.pro.user __pycache__

# Regenerate implicit dependencies.
ifneq ($(if $(MAKECMDGOALS),$(filter-out clean listvars cxxmacros,$(MAKECMDGOALS)),true),)
    -include $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.d,$(SOURCES))
endif
$(BINDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) -MM $(CPPFLAGS) -MT $(BINDIR)/$*.o -MT $@ $< >$@ || rm -f $@

# Display make variables for debug purposes.
listvars:
	@true
	$(foreach v, \
	  $(sort $(filter-out .% ^% @% _% *% \%% <% +% ?% BASH% LS_COLORS SSH% VTE% XDG% F_%,$(.VARIABLES))), \
	  $(info $(v) = "$($(v))"))

# Display C++ predefined macros for debug purposes.
cxxmacros:
	@$(CPP) $(CXXFLAGS) -x c++ -dM /dev/null | sort
