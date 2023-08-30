#!/usr/bin/env bash
#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Run cryptobench test, also report system characteristics.
#----------------------------------------------------------------------------

trim() { sed -e 's/^[ \t\n\r]*//' -e 's/[ \t\n\r]*$//' -e 's/[ \t\n\r][ \t\n\r]*/ /'; }
firstvalue() { head -1 | sed 's/.*://' | trim; }

SYSTEM=$(uname -s)
SYSTEM=${SYSTEM/Darwin/macOS}
ARCH=$(uname -m)
ARCH=${ARCH/aarch64/arm64}
KERNEL=$(uname -p -r -v )

DISTRO=
if [[ $SYSTEM == macOS ]]; then
    DISTRO="$(echo $(sw_vers --productName) $(sw_vers --productVersion) $(sw_vers --productVersionExtra) $(sw_vers --buildVersion) | trim)"
elif [[ -n $(which lsb_release) ]]; then
    DISTRO=$(lsb_release -d | firstvalue)
else
    for name in os fedora redhat ubuntu debian alpine; do
        if [[ -e /etc/$name-release ]]; then
            DISTRO=$(head -1 /etc/$name-release | trim)
            [[ -n $DISTRO ]] && break
        fi
    done
fi

CPU=
if [[ $SYSTEM == macOS ]]; then
    CPU=$(sysctl machdep.cpu.brand_string | firstvalue)
elif [[ -e /proc/cpuinfo ]]; then
    CPU=$(grep -i '^model name' /proc/cpuinfo | firstvalue)
    HW=$(grep -i '^hardware' /proc/cpuinfo | firstvalue)
    MD=$(grep -i '^model' /proc/cpuinfo | firstvalue)
    [[ -n $HW ]] && CPU="$CPU, $HW"
    [[ -n $MD ]] && CPU="$CPU, $MD"
fi
    
echo "arch: $ARCH"
echo "cpu: $CPU"
echo "system: $SYSTEM"
echo "distro: $DISTRO"
echo "kernel: $KERNEL"
echo "date: $(date +%Y-%m-%d)"

cd $(dirname "$0")
make -j4 && build/cryptobench --min-time 5
