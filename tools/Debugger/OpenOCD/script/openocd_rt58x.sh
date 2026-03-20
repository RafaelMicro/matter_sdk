#!/usr/bin/env bash
set -e

# Usage:
#   ./openocd_rt5xx.sh rt582
#   ./openocd_rt5xx.sh rf1301
# Default: rt584

SOC="${1:-rt584}"

GDB_PORT=50000
TCL_PORT=50001
TELNET_PORT=50002

WORKSPACE_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
OPENOCD_BIN="$WORKSPACE_DIR/OpenOCD/bin/linux/openocd"
SCRIPT_DIR="$WORKSPACE_DIR/OpenOCD/script"

# --- SoC -> target cfg mapping ---
case "$SOC" in
  rt581|rt582|rt583)
    TARGET_CFG="target/rt58x.cfg"
    ;;
  rt584l|rt584h|rt584ha4|rf1301)
    TARGET_CFG="target/rt584.cfg"
    ;;
  *)
    echo "[ERROR] Unknown SoC: $SOC"
    echo "        Supported: rf581 rt582 rt583 rt584h rt584l rt584ha4 rf1301"
    exit 2
    ;;
esac

echo "========================================"
echo "[OpenOCD] SoC        : $SOC"
echo "[OpenOCD] Target CFG : $TARGET_CFG"
echo "[OpenOCD] BIN        : $OPENOCD_BIN"
echo "[OpenOCD] SCRIPT DIR : $SCRIPT_DIR"
echo "[OpenOCD] Ports      : gdb=$GDB_PORT tcl=$TCL_PORT telnet=$TELNET_PORT"
echo "========================================"

exec sudo "$OPENOCD_BIN" \
  -s "$SCRIPT_DIR" \
  -f interface/cmsis-dap.cfg \
  -f "$TARGET_CFG" \
  -c "gdb port ${GDB_PORT}; tcl port ${TCL_PORT}; telnet port ${TELNET_PORT}"
