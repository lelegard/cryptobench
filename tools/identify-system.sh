#!/usr/bin/env bash
#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Identify the running system, similar output format as cryptobench.
#----------------------------------------------------------------------------

trim() { tr '\n' ' ' | tr '\r' ' ' | tr '\t' ' ' | sed -e 's/^ *//' -e 's/ *$//' -e 's/  */ /'; }
firstvalue() { head -1 | sed 's/.*://' | trim; }

SYSTEM=$(uname -s)
SYSTEM=${SYSTEM/Darwin/macOS}
ARCH=$(uname -m)
ARCH=${ARCH/aarch64/arm64}
KERNEL=$(uname -p -r -v )

DISTRO=
if [[ $SYSTEM == macOS ]]; then
    DISTRO="$(echo $(sw_vers -productName) $(sw_vers -productVersion) $(sw_vers -buildVersion) | trim)"
elif [[ -n $(which lsb_release 2>/dev/null) ]]; then
    DISTRO=$(lsb_release -d | firstvalue)
else
    for name in fedora redhat ubuntu debian alpine os; do
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
    MN=$(grep -i '^model name' /proc/cpuinfo | firstvalue)
    HW=$(grep -i '^hardware' /proc/cpuinfo | firstvalue)
    MD=$(grep -i '^model' /proc/cpuinfo | firstvalue)
    CPU=$(trim <<<"$MN $HW $MD")
    CPU=${CPU:-unknown}
fi

# Raspberry Pi incorrectly identifies its CPU.
[[ $CPU == *BCM2835*Raspberry*Pi*4* ]] && CPU=${CPU/BCM2835/BCM2711}

# The item "cpu-index:", if not empty, is a short string to use instead of "cpu:" in
# summary tables. Useful if the "cpu:" string is too long. Can be manually set in the
# report file afterward or can be preset in environment variable CRYPTOBENCH_CPU_INDEX.

# The item "machine:" is informative only. It gives the machine model name. It can be
# manually set in the report file afterward or can be preset in environment variable
# CRYPTOBENCH_MACHINE

echo "system: arch: $ARCH"
echo "system: cpu: $CPU"
echo "system: cpu-index: $CRYPTOBENCH_CPU_INDEX"
echo "system: machine: $CRYPTOBENCH_MACHINE"
echo "system: frequency: $CRYPTOBENCH_FREQUENCY"
echo "system: os: $SYSTEM"
echo "system: distro: $DISTRO"
echo "system: kernel: $KERNEL"
echo "system: date: $(date +%Y-%m-%d)"
