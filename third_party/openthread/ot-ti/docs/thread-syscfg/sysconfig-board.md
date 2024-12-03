# Configure The Board Files with SysConfig 

## Table of Contents

* [Reserve Peripherals](#Reserve-Peripherals)
* [Hardware View](#Hardware-View)
* [ Migrating Board Variant](#Migrating-Board-Variant)

The board view shows you what pins are available on your board, and what
peripherals are assigned to what pins. Hover over each pin for more
information.


![](resources/board_view.png)

Enabled and configured peripherals are displayed with a green symbols.
To add a new peripheral, press the plus sign next to the peripheral
type. The \"Show generated files\" tab lets you see the files generated
based on SysConfig. Generated files are dynamically updated every time
you make a change in SysConfig. When you build your project, the
generated files are exported into the output folder of your project.

![](resources/show_generated_files_tab.png)

## Reserve Peripherals 

Certain use cases may require preventing SysConfig from generating the
configuration of specific peripherals.

Examples of use cases requiring to reserve peripherals are:

-   dynamic re-configuration of a peripheral
-   leveraging the Sensor Controller (on devices enabling it)
-   already handling certain peripherals configuration
-   with hardware constraints preventing to use certain peripherals
    (typically GPIOs)
-   peripherals reserved for future use

Below are the steps required to utilize the Reserve Peripherals feature.

1.  Open the `.syscfg` file using the SysConfig GUI

2.  Open the panel \"Reserve Peripherals\"

    ![](resources/syscfg-reserve-peripheral-pannel.png)

3.  Reserve the resources

    Reserve a peripheral using SysConfig. This image shows how
    to reserve DIO 15

    <div style="text-align: center;">
    <img src="resources/syscfg-reserve-gpio.png"
    alt="resources/syscfg-reserve-gpio.png" />
    </div>


SysConfig ensures no resource conflict occurs. In case no solution can
be found, an error is raised.

## Hardware View 

The Hardware view may be used to add software modules for supporting
hardware resources on EVMs or LaunchPads. The same capability could be
done in the Software view by adding a hardware. However the Hardware
view provides a hardware-first perspective to show which software could
be used with hardware resources.

> Sysconfig Hardware View
> <div style="text-align: center;">
> <img src="resources/syscfg_hardware_view.png"
> alt="/sysconfig/resources/syscfg_hardware_view.png" />
> </div>

also contains a SysConfig training module for most of the platform SDKs
in the Tools folder.
