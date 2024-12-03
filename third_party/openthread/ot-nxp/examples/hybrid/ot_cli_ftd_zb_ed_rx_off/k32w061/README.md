# Dual mode app - OpenThread FTD & ZB ZED RX OFF

This directory contains the code of the dual mode application Openthread FTD + ZB ZED RX OFF for K32W061

## Toolchain & Tool

Follow the same procedure as described in the [README.md][k32w061_readme], describing what need to be install to run OpenThread on NXP K32W061 platform.

[k32w061_readme]: ../../../../src/k32w0/k32w061/README.md

## Building the examples

To build the example, follow what is described in [README.md 'Building the examples section'][k32w061_readme_build_example].

[k32w061_readme_build_example]: ../../../../src/k32w0/k32w061/README.md#Building-the-examples

Instead of running (which would build all k32w061 examples)

```bash
$ ./script/build_k32w061
```

it is possible to build only ot_ftd_uart_zb_ed_rx_off examples.

The below ot_ftd_uart_zb_ed_rx_off targets can be generated:

- `ot_ftd_uart_zb_ed_rx_off` with FTD app running on USART0 and ZB app running on USART1

The following command can be used to build only a specific target:

```bash
$ ./script/build_k32w061 <target_name>
```

After a successful build, the application binary will be generated in `build_k32w061/<target_name>/ot-ftd-uart-zb-ed-rx-off-k32w061`.

## Flash Binaries

To flash the binary, follow the same procedure as described in the "Flash Binaries" section in [README.md][k32w061_readme_flash_binaries].

[k32w061_readme_flash_binaries]: ../../../../src/k32w0/k32w061/README.md#Flash-Binaries
