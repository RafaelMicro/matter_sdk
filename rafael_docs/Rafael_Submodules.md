# Rafael Submodules

This project uses two Git submodules from Rafael, managing the OpenThread protocol stack and peripheral component drivers respectively.

## 1. OpenThread (`third_party/openthread/openthread-rafael`)

- **Remote**: `https://github.com/RafaelMicro/openthread`
- **Path**: `third_party/openthread/openthread-rafael/`
- **Description**: Rafael's customized OpenThread implementation, containing:
  - `openthread/` — OpenThread core source code
  - `openthread_port/` — Rafael platform port layer

## 2. Rafael SDK Components (`third_party/rafael/repo/components`)

- **Remote**: `https://github.com/RafaelMicro/components.git`
- **Path**: `third_party/rafael/repo/components/`
- **Description**: Rafael peripheral component drivers and platform support libraries
