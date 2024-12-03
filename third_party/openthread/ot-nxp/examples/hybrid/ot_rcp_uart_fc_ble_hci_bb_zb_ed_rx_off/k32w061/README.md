# Tri radio - OpenThread RCP, BLE HCI BB and ZED RX OFF example on K32W061

This directory contains the code of the tri radio application Openthread RCP + BLE HCI BB + ZED RX OFF for K32W061

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

it is possible to build only ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off examples.

The below ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off targets can be generated:

- `ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off` with spinel running on one UART and HCI over spinel using spinel_vendor_hook and ZB ZED on another UART.

The following command can be used to build only a specific target:

```bash
$ ./script/build_k32w061 <target_name>
```

After a successful build, application binaries will be generated in `build_k32w061/<target_name>/bin` and would contain the file called "ot-rcp-ble-hci-bb-k32w061.elf.bin.h".

For each target build, another build will be automatically triggered to generate the same version with PTA support. As an example when the target `ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off` will be used 2 folders called `ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off` and `ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off_pta` will be created.

## Flash Binaries

Note: this section should not be followed if the RCP binary is going to be used with an host device supporting the `The Over The Wire (OTW) protocol (over UART)`.

To flash the binary, follow the same procedure as described in the "Flash Binaries" section in [README.md][k32w061_readme_flash_binaries].

[k32w061_readme_flash_binaries]: ../../../../src/k32w0/k32w061/README.md#Flash-Binaries
