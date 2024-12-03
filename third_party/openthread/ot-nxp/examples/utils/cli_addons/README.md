# ot-cli addons

NXP's ot-cli example includes an addons mechanism which allow to easily register extra commands to ot-cli.

By default, no addons are built, but they can be enabled by settings CMake options.

## iperf-cli addon

[iPerf](https://github.com/esnet/iperf) is a tool used to measure throughput performances on IP networks.

The tool has been ported to ot-cli is order to run benchmarks and throughput scenarios over OpenThread networks.

**Note:** This addon is based on "lwiperf" which is a port of iPerf onto lwIP tcp/ip stack.

To build ot-cli with iperf addon, following CMake flags are required:

`OT_APP_CLI_FREERTOS_IPERF`: Adds iperf_cli addon to the ot-cli application

`OT_NXP_LWIP`: Builds NXP's lwIP as a third_party library

`OT_NXP_LWIP_IPERF`: Adds lwiperf port to NXP's lwIP library

Example of a typical command:

```bash
./script/build_<platform> -DOT_APP_CLI_FREERTOS_IPERF=ON -DOT_NXP_LWIP=ON -DOT_NXP_LWIP_IPERF=ON
```

## lwip-cli addon

Allows following:

- Add/remove of IPv6 addresses.
- Reading MAC addresses.
- Read of link state.

Enable addon using `-DOT_APP_CLI_FREERTOS_LWIP=ON` compilation parameter.

Type `lwip help` to the cli to get list of all available commands.

## wifi-cli addon

Allows connecting/disconnecting from Wi-Fi network.

Enable addon using `-DOT_APP_CLI_FREERTOS_WIFI=ON` compilation parameter.

Type `wifi help` to the cli to get list of all available commands.
