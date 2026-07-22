#!/usr/bin/env bash
# Shim for cortex-debug: run the pigweed-venv arm-none-eabi-gdb wrapper.
# The wrapper requires arm-none-eabi-gcc on PATH to locate the toolchain's
# bundled Python 3.8, so prepend the CIPD ARM bin dir regardless of how
# VSCode was launched.
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export PATH="${REPO_ROOT}/.environment/cipd/packages/arm/bin:${PATH}"
exec "${REPO_ROOT}/.environment/pigweed-venv/bin/arm-none-eabi-gdb" "$@"
