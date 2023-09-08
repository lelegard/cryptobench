#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Top-level makefile for the cryptobench tool.
#----------------------------------------------------------------------------

MAKEFLAGS += --no-print-directory

default clean:
	@$(MAKE) $@ -C src
