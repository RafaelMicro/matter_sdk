---
name: binary-size-comparison
description:
    Guidelines for analyzing and comparing RT58x binary (ELF) sizes — section
    totals, symbol-level diffs between two builds, top flash/RAM consumers, and
    per-library footprint from the linker map. Use when evaluating the
    flash/RAM impact of a code change or hunting size regressions.
---

# RT58x Binary Size Comparison Workflow

All commands were verified against a real build (`out/lighting-app/RT583/`).
Replace paths for your app/board. Unlike the upstream skill, no container and
no Rust tools are required — everything runs on the host with in-repo scripts.

## 1. Build Artifacts

`./scripts/examples/gn_rt58x_example.sh <AppRoot> <outFolder> <BOARD>` builds
into `<outFolder>/<BOARD>/` and produces:

```
out/<app>/<BOARD>/
├── <BOARD>-<app>-example.out        # the ELF
├── <BOARD>-<app>-example.out.map    # linker map (-Wl,-Map)
├── <BOARD>-<app>-example.bin        # objcopy output for flashing
└── size.txt                         # arm-none-eabi-size -A (written by build script)
```

Toolchain binaries: on PATH after `source scripts/activate.sh`, or directly at
`.environment/cipd/packages/arm/bin/arm-none-eabi-{size,nm,objdump}`:

```bash
ARM=.environment/cipd/packages/arm/bin
```

## 2. Quick Section Totals

```bash
cat out/<app>/<BOARD>/size.txt        # written by the build script
# or regenerate:
$ARM/arm-none-eabi-size -A out/<app>/<BOARD>/<BOARD>-<app>-example.out
```

Interpretation:

- **Flash usage** ≈ `.text` (code + rodata) + `.data` (load image) +
  `.cli_cmd_pool` + `.ARM.exidx`
- **RAM usage** ≈ `.data` + `.bss` + `.heap` + `.stack_dummy` (the
  `__STACK_SIZE` region)
- `.debug_*` sections are huge but live only in the ELF, not on the device.

Quick compare of two builds: keep the old `size.txt` and
`diff old_size.txt size.txt`.

Memory regions (budget ceilings) are defined per board in
`examples/platform/rt58x/ldscript/*.ld`; print the actual regions from a map:

```bash
sed -n '/^Memory Configuration/,/^Linker script and memory map/p' <map>
```

Example (RT583): `LMA_CORE` (app flash) `0x127000` (~1180K), `VMA_DATA` (RAM)
`0x23800` + `VMA_STACK` `0x800`. RT584/RT584HA4: flash at `0x10000000+`, RAM at
`0x30000000` (`__RAM_SIZE = 192K`). The ninja link step also prints region
usage (`-Wl,--print-memory-usage`).

## 3. Symbol-Level Diff Between Two Builds (main workflow)

Use the in-repo `scripts/tools/binary_elf_size_diff.py`. It reads symbols via
`nm` (host GNU nm handles ARM ELF fine) and reports ADDED / REMOVED / CHANGED
per function.

### A. One-time Python deps

The script needs a few packages not present in the pigweed venv by default:

```bash
.environment/pigweed-venv/bin/pip install cxxfilt tabulate coloredlogs plotly
```

(If `uv` is available, `uv run --script scripts/tools/binary_elf_size_diff.py ...`
handles deps automatically instead.)

### B. Build the baseline and save its ELF

The build overwrites `out/<app>/<BOARD>/`, so copy the baseline ELF out first.

```bash
CURRENT_COMMIT=$(git rev-parse HEAD)
BASELINE=$(git merge-base master HEAD)   # or any commit/branch to compare against

git checkout $BASELINE
./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583

mkdir -p out/size-compares
cp out/lighting-app/RT583/RT583-lighting-app-example.out out/size-compares/baseline.out
cp out/lighting-app/RT583/size.txt out/size-compares/baseline-size.txt

git checkout $CURRENT_COMMIT
```

For uncommitted working-tree changes, use `git stash` / `git stash pop` around
the baseline build instead of checking out commits.

### C. Build the updated branch and diff

```bash
./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583

.environment/pigweed-venv/bin/python scripts/tools/binary_elf_size_diff.py \
    out/lighting-app/RT583/RT583-lighting-app-example.out \
    out/size-compares/baseline.out
```

Positive sizes = growth in the first (updated) binary. Useful flags:
`--output csv --skip-total` for machine-readable output. See
`scripts/tools/ELF_SIZE_TOOLING.md` for sankey export and more examples.

## 4. Top Symbols by Size (single binary)

```bash
# Top RAM consumers (.bss = b/B, .data = d/D):
$ARM/arm-none-eabi-nm --size-sort -C -S <elf> | grep -i ' [bd] ' | tail -20
# Top Flash consumers (.text = t/T, .rodata = r/R):
$ARM/arm-none-eabi-nm --size-sort -C -S <elf> | grep -i ' [tr] ' | tail -20
```

Typical top RAM symbols on RT583 lighting-app: `ot::gInstanceRaw` (~15K),
`chip::Server::sServer` (~12.5K), `msg_data` (~9K, CHIP packet buffer pool),
`appStack`, singletons (`ThreadStackManagerImpl`, `sInteractionModelEngine`).
FreeRTOS task stacks created with `xTaskCreateStatic` show up as their static
buffer symbols; `xTaskCreate` stacks live inside the FreeRTOS heap and are NOT
visible in `nm` — check `configTOTAL_HEAP_SIZE`/heap_5 regions instead.

## 5. Per-Library / Per-Object Footprint from the Map

GNU ld maps wrap long section names onto two lines, and everything before
`Linker script and memory map` is discarded (garbage-collected) sections. Both
must be handled or the numbers will be wrong:

```bash
awk '
/^Linker script and memory map/ { live=1 }
!live { next }
/^ (\.text|\.rodata|\.data|\.bss|COMMON)/ {
  if (NF==1) { getline cont; split(cont, f, " "); size=strtonum(f[2]); obj=f[3] }
  else       { size=strtonum($3); obj=$4 }
  if (obj ~ /\.(a|o)(\(|$)/) { sub(/\(.*/,"",obj); n=split(obj,p,"/"); sum[p[n]]+=size }
}
END { for (l in sum) printf "%10d  %s\n", sum[l], l }' <map> | sort -rn | head -20
```

Verified output (RT583 lighting-app): `libCHIP.a` ~533K, `libCM3_ble_host.a`
~83K (Rafael BLE stack), `sdk.rt569mp_fw.c.o` ~68K (RF firmware blob),
`libCHIPAppServer.a` ~34K, then per-cluster objects.

Variants — change the section regex on the match line:

- Flash only: `/^ (\.text|\.rodata|\.data)/`
- RAM only: `/^ (\.data|\.bss|COMMON)/`

To attribute one symbol, search the map directly (the address/size may be on
the line after the section name due to wrapping):

```bash
grep -A1 'gInstanceRaw' <map>
```

## 6. Interactive Treemap (humans only)

`scripts/tools/file_size_from_nm.py` renders an interactive plotly treemap of
namespace/method sizes:

```bash
.environment/pigweed-venv/bin/python scripts/tools/file_size_from_nm.py \
    --zoom '::chip::app' \
    out/lighting-app/RT583/RT583-lighting-app-example.out
```

> [!WARNING] This opens a browser and blocks the terminal — do NOT run it from
> an AI agent or non-interactive session. Agents should use sections 3–5
> instead. (Also needs `numpy` and `pandas` in the venv.)

## 7. Pitfalls Checklist

1. Use the `.out` ELF for all analysis — `.bin` is the flash image (no
   symbols, not usable for these tools).
2. In map analysis, skip everything before `Linker script and memory map`
   (discarded sections) and handle two-line wrapped entries.
3. `.text` in the final output includes `.rodata` input sections on this
   platform; don't double-count.
4. `*fill*` (alignment padding) is not counted by the awk above; totals may be
   slightly below `size -A` values.
5. Build both binaries with identical options (`--sed`, `--ota`, `--certs`,
   same BOARD) or the diff will be dominated by config noise.
