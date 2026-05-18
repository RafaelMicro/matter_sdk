# Building Matter For Linux/MacOS

## Building Matter

-   The open terminal uses the following command to clone the project, and runs
    the following command to sync the submodule:
    ```
    $ git clone --recurse-submodules https://github.com/RafaelMicro/matter_sdk.git
    $ cd matter_sdk
    $ git submodule update --init
    ```

---

## Prerequisites

-   Before building, you'll need to install a few OS-specific dependencies.
### For Linux:
-   On Debian-based Linux distributions such as Ubuntu, these dependencies
        can be satisfied with the following:
    ```
    sudo apt-get install git gcc g++ pkg-config cmake libssl-dev libdbus-1-dev \
        libglib2.0-dev libavahi-client-dev ninja-build python3-venv python3-dev \
        python3-pip unzip libgirepository1.0-dev libcairo2-dev libreadline-dev \
        default-jre
    ```
#### Upgrading Python on Ubuntu 22.04

-   Ubuntu 22.04 ships with Python 3.10 by default, but Matter SDK requires Python
    3.11 or newer. To upgrade Python, run the following commands:

    ```shell
    sudo apt-get install python3.11 python3.11-dev python3.11-venv
    # Register python3.10 so that it can be switched back if needed
    sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1
    # Register python3.11 with higher priority (will be automatically selected)
    sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 2
    # Verify that python3 points to python3.11
    python3 --version
    ```
### For macOS:
-   On macOS, first install Xcode from the Mac App Store. The remaining
        dependencies can be installed and satisfied using
        [Brew](https://brew.sh/):
    ```
    $ brew install openssl pkg-config
    ```
-   However, that does not expose the package to **pkg-config**. To fix
        that, one needs to run something like the following:
-   Intel:
        ```
        $ cd /usr/local/lib/pkgconfig
        $ ln -s ../../Cellar/openssl@1.1/1.1.1g/lib/pkgconfig/* .
        ```
        where openssl@1.1/1.1.1g may need to be replaced with the actual version
        of OpenSSL installed by Brew.
-   Apple Silicon:
        ```
        $ export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:"/opt/homebrew/opt/openssl@3/lib/pkgconfig"
        ```
        Note: If using MacPorts, port install openssl is sufficient to satisfy
        this dependency.

---

## Prepare for building

Before running any other build command, the scripts/activate.sh environment
setup script should be sourced at the top level. This script takes care of
downloading GN, ninja, and setting up a Python environment with libraries used
to build and test.

```
$ source scripts/activate.sh
```

If this script says the environment is out of date, it can be updated by
running:

```
$ source scripts/bootstrap.sh
```

The **scripts/bootstrap.sh** script re-creates the environment from scratch,
which is expensive, so avoid running it unless the environment is out of date.
![](https://hackmd.io/_uploads/Hk-loXD43.png)

---

## Building

### Building project

Run the following command to build a project:
```
$ ./scripts/examples/gn_rt58x_example.sh ./examples/<project-name>/rt58x ./out/<project-name> <board> [--sed=<yes|no>] [--ota=<yes|no>] [--certs=<yes|dac-only|no>]
```

**Arguments:**

| Argument | Possible Values |
|---|---|
| `<project-name>` | `contact-sensor-app`, `humidity-sensor-app`, `light-switch-app`, `lighting-app`, `lighting-zigbee`, `lock-app`, `occupancy-sensor-app`, `smart-plug-app`, `smoke-alarm-app`, `temperature-measurement-app`, `thermostat`, `water-valve-app`, `window-app` |
| `<board>` | `RT583`, `RT584H`, `RT584L`, `RT584HA4` |
| `--sed` | `yes` / `no` (default: `no`) — Sleep End Device support |
| `--ota` | `yes` / `no` (default: `no`) — OTA update support |
| `--certs` | `yes` / `dac-only` / `no` (default: `no`) — Attestation credentials |

Example:
```
$ ./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583 --sed=yes --ota=yes --certs=yes
```

> **VSCode Task Explorer**: If you have the
> [Task Explorer](https://marketplace.visualstudio.com/items?itemName=spmeesseman.vscode-taskexplorer)
> extension installed, you can use the **`Matter: Build Example`** task instead.
> It will prompt you to select the project, board, and optional features
> interactively.

### Removing build artifacts

If you're planning to build the example for a different kit or make changes to
the configuration, remove all build artifacts before building. To do so, use the
following command:

```
$ sudo rm -fr ./out/<project-name>
```

> **VSCode Task Explorer**: Alternatively, run the
> **`Matter: Clean Build Output`** task, which will prompt you to
> select the project and board to clean up.
