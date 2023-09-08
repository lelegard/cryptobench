#!/usr/bin/env bash
#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Run cryptobench test, also report system characteristics.
#----------------------------------------------------------------------------

cd $(dirname "$0")/..
tools/identify-system.sh
make -j4 -C src && build/cryptobench --min-time 5
