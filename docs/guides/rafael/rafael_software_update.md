# RafaelMicro RT583 Matter OTA example

## Building Matter OTA example

-   On a Linux platform build the lighting-app with OTA feature as follow:

        ./scripts/examples/gn_rt582_example.sh examples/lighting-app/rt582/ out RT583 --ota=yes

## Running the OTA Download scenario

-   On a Linux platform create the Matter OTA image using Rafael OTA tool:

        ./src/app/rafael_ota_tool.py create -v 0xFFF1 -p 0x8005 -vn 2 -vs "2.0" -da sha256 out/RT583/chip-rt582-lighting-app-example.bin out/RT583/chip-rt582-lighting-app-example.ota

-   If the RT583 had been previously commissioned, please hold Button SW2 for
    6 seconds to factory-reset the device.

-   In a Raspberry Pi terminal 2 enter following command with operationalDataset where is obtained from the Thread Border Router. :

        ./chip-tool pairing ble-thread 1 hex:<operationalDataset> 20202021 3840
        
-   In a Raspberry Pi terminal 1 start the Provider app passing to it the path to the Matter
    OTA image created in the previous step:

        rm -r /tmp/chip_*
        
        ./chip-ota-provider-app -f chip-rt582-lighting-app-example.ota --discriminator 3841

-   In a Raspberry Pi terminal 2 run the chip-tool commands to provision the Provider:

        ./chip-tool pairing onnetwork-long 2 20202021 3841

        ./chip-tool accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [112233], "targets": null}, {"fabricIndex": 1, "privilege": 3, "authMode": 2, "subjects": null, "targets": null}]' 2 0


-   Once the commissioning process completes enter:

        ./chip-tool otasoftwareupdaterequestor announce-ota-provider 2 0 0 0 1 0

-   The application device will connect to the Provider and start the image
    download. Once the image is downloaded the device will reboot into the
    downloaded image.

## Managing the Software Version

In order for the Provider to successfully serve the image to a device during the
OTA Software Update process the Software Version parameter that the .ota file
was built with must be greater than the
CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION parameter set in the application's
`CHIPProjectConfig.h` file. The Software Version parameter is set by the `-vn`
parameter passed to the `rafael_ota_tool.py create` command. For example, if the
application's running image was built with
CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION set to 1 and if the `.ota` file is
built with `-vn 2` then the Provider will serve the update image when requested.

In order for the OTA Software Update subsystem to consider an update to be
successful and for the NotifyUpdateApplied command to be transmitted the
CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION in the updated image must exceed the
software version of the running image (continuing the above example, the image
for the update must be built with CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION set
to 2).
