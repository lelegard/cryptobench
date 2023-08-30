#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# General makefile
#----------------------------------------------------------------------------

default: exec

SHELL     = /usr/bin/env bash --noprofile
SYSTEM   := $(shell uname -s)
CXXFLAGS += -std=c++17 -Werror -Wall -Wextra -Wno-unused-parameter $(if $(DEBUG),-g,-O2)
CPPFLAGS += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include))
LDFLAGS  += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib)) $(if $(DEBUG),-g)
LDLIBS   += -ltomcrypt -ltommath -lgnutls -lmbedcrypto -lcrypto -lgmp -lm
NM        = nm
NMFLAGS   = $(if $(findstring Linux,$(SYSTEM)),-D)

SRCDIR    = src
BINDIR    = build
EXEC      = $(BINDIR)/cryptobench
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

# Check if LibTomCrypt is built with support for LibTomMath and GMP.
# On most distros, it is built with both. But there are exceptions.
# Example: On Fedora 38, LibTomCrypt does not support GMP.
LIBTOM = $(wildcard /usr/lib*/libtomcrypt.so /usr/lib*/*/libtomcrypt.so /opt/homebrew/lib/libtomcrypt.dylib /usr/local/lib/libtomcrypt.dylib)
LIBTOMSYMS = $(shell $(NM) $(NMFLAGS) $(LIBTOM) 2>/dev/null | grep -e ltm_desc -e gmp_desc)

CPPFLAGS += $(if $(findstring ltm_desc,$(LIBTOMSYMS)),-DLTM_DESC)
CPPFLAGS += $(if $(findstring gmp_desc,$(LIBTOMSYMS)),-DGMP_DESC)

# Build operations.
exec: $(EXEC)
	@true
$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
clean:
	rm -rf build build-* core *.tmp *.log src/*.pro.user

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
