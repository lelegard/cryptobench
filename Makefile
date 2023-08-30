#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# General makefile
#----------------------------------------------------------------------------

default: exec

SHELL     = /usr/bin/env bash --noprofile
CXXFLAGS += -std=c++17 -O2 -Werror -Wall -Wextra -Wno-unused-parameter
CPPFLAGS += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include)) -DLTM_DESC -DGMP_DESC
LDFLAGS  += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib))
LDLIBS   += -ltomcrypt -ltommath -lgnutls -lmbedcrypto -lcrypto -lgmp -lm

SRCDIR    = src
BINDIR    = build
EXEC      = $(BINDIR)/cryptobench
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

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
