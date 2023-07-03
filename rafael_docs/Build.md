# Rafael matter environment setup for windows

## Prerequest

-   Visual Studio Code: [Download](https://code.visualstudio.com/Download)
-   Docker Desktop:
    [Download](https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe?utm_source=docker&utm_medium=webreferral&utm_campaign=dd-smartbutton&utm_location=module)
-   Git bash: [Download](https://git-scm.com/download/win)

---

## Building Matter

In windows path D:\workspace, use Git bash To check out the Matter repository:

```
git clone --recurse-submodules https://github.com/RafaelMicro/matter_sdk.git
```

If you already have a checkout, run the following command to sync submodules:

```
git submodule update --init
```

---

## Setup environment

### Install VSCode extension

Install all recommendations tool:

-   spmeesseman.vscode-taskexplorer
-   ms-vscode.cpptools
-   ms-vscode-remote.remote-containers
-   ms-azuretools.vscode-docker
-   dan-c-underwood.arm
-   twxs.cmake
-   vadimcn.vscode-lldb
-   marus25.cortex-debug

![Vscode Recommand](./images/recommand.png)

### Install VSCode extension

Run VSCode task :"RT matter dev setup" Task "RT matter dev setup" include three
sub-task:

1. "RT matter image build (Step I)"
2. "RT matter container build (Step II)"
3. "RT matter container env setup (Step III)"

![Vscode TASK](./images/Task.png)

### Building project

Run VSCode task :"Build RT lighting example"
