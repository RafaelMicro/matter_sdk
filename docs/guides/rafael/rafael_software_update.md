# RafaelMicro RT583 Matter OTA example

## Building Matter OTA example

- On a Linux platform build the lighting-app with OTA feature as follow

      ./scripts/examples/gn_rt582_example.sh examples/lighting-app/rt582/ out RT583 --ota=yes

## Create Matter OTA file
- On a Linux platform create the Matter OTA file using Rafael OTA tool

      ./src/app/rafael_ota_tool.py create -v 0xFFF1 -p 0x8005 -vn 2 -vs "2.0" -da sha256 out/RT583/chip-rt582-lighting-app-example.bin out/RT583/chip-rt582-lighting-app-example.ota
        
- To verify the Matter OTA file, run the script with following command
        
      ./src/app/rafael_ota_tool.py show out/RT583/chip-rt582-lighting-app-example.ota

- Example output

      Magic: 1beef11e
      Total Size: 443856
      Header Size: 63
      Header TLV:
         [0] Vendor Id: 65521 (0xfff1)
         [1] Product Id: 32773 (0x8005)
         [2] Version: 2 (0x2)
         [3] Version String: 2.0
         [4] Payload Size: 443777 (0x6c581)
         [8] Digest Type: 1 (0x1)
         [9] Digest: 7828284dfc715ecd89292b03b798cca37c87301cf84691292e52e12eab6990b9

## Running the OTA Download scenario

- In a Raspberry Pi open a terminal 1 and start the Matter OTA Provider app passing the to it the path to the Matter OTA file created in the previous step

      rm -r /tmp/chip_*
        
      ./chip-ota-provider-app -f chip-rt582-lighting-app-example.ota --discriminator 3841

- In a Raspberry Pi terminal 1 run the chip-tool commands to provision the Provider

      ./chip-tool pairing onnetwork-long 2 20202021 3841

      ./chip-tool accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [112233], "targets": null}, {"fabricIndex": 1, "privilege": 3, "authMode": 2, "subjects": null, "targets": null}]' 2 0

- If the RT583 had been previously commissioned, please hold Button SW2 for
    6 seconds to factory-reset the device
    
- In a Raspberry Pi open a terminal 2 and enter following command with operationalDataset where is obtained from the OpenThread Border Router

      ./chip-tool pairing ble-thread 1 hex:<operationalDataset> 20202021 3840

- Once the commissioning process completes enter

      ./chip-tool otasoftwareupdaterequestor announce-ota-provider 2 0 0 0 1 0

- The application device will connect to the Provider and start the image
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
