# Dual mode app - OpenThread RCP & ZB ZC

This directory contains the code of the dual mode application Openthread RCP + ZB coordinator for K32W1

## Toolchain & Tool

Follow the same procedure as described in the [README.md][k32w1_readme], describing what need to be install to run OpenThread on NXP K32W1 platform.

[k32w1_readme]: ../../../../src/k32w1/README.md

## Building the examples

To build the example, follow what is described in [README.md 'Building the examples section'][k32w1_readme_build_example].

[k32w1_readme_build_example]: ../../../../src/k32w1/README.md#Building-the-examples

Instead of running (which would build all k32w1 examples)

```bash
$ ./script/build_k32w1
```

it is possible to build only ot_rcp_uart_dma_zb_zc examples.

The below ot_rcp_uart_dma_zb_zc targets can be generated:

- `ot_rcp_uart_dma_zb_zc` with RCP app running on LPUART1 with DMA and ZB app running on LPUART0.

The following command can be used to build only a specific target:

```bash
$ ./script/build_k32w1 <target_name>
```

After a successful build, the application binary will be generated in `build_k32w1/<target_name>/ot-rcp-uart-dma-zb-zc-k32w1`.

## Flash Binaries

To flash the binary, follow the same procedure as described in the "Flash Binaries" section in [README.md][k32w1_readme_flash_binaries].

[k32w1_readme_flash_binaries]: ../../../../src/k32w1/README.md#Flash-Binaries
