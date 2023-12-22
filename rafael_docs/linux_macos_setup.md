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
-   For Linux:
    -   On Debian-based Linux distributions such as Ubuntu, these dependencies
        can be satisfied with the following:
    ```
    $ sudo apt-get install git gcc g++ pkg-config libssl-dev libdbus-1-dev \
         libglib2.0-dev libavahi-client-dev ninja-build python3-venv python3-dev \
         python3-pip unzip libgirepository1.0-dev libcairo2-dev libreadline-dev
    ```
-   For macOS:
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

Run the following command to build a project: 1. Select target board
(RT583) 2. Select OTA support or not 3. Select sleep end device (SED)
support or not 4. Select certs support or not

```
$ ./scripts/examples/gn_rt58x_example.sh ./examples/<project-name>/rt58x ./out/<project-name> <Board(RT583)> <optional>
Example:
$ ./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583
$ ./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583 --sed=yes --ota=yes
$ ./scripts/examples/gn_rt58x_example.sh ./examples/lighting-app/rt58x ./out/lighting-app RT583 --sed=yes --ota=yes --certs=yes
```

### Removing build artifacts

If you're planning to build the example for a different kit or make changes to
the configuration, remove all build artifacts before building. To do so, use the
following command:

```
$ sudo rm -fr ./out/<project-name>
```
