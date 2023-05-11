# Commissioning

Using CHIP Tool for Matter device testing, Setup step:

1.  **Prepare the Rafael Matter device**
2.  **Enable Bluetooth LE advertising on Matter device**: Examples are
    configured to advertise automatically on boot.
3.  **Make sure the IP network is set up**: To follow the next steps, the IP
    network must be up and running.
4.  **Determine network pairing credentials** Get Thread network credentials:
    ```
    $ sudo ot-ctl dataset active -x
    0e080000000000010000000300001335060004001fffe002084fe76e9a8b5edaf50708fde46f999f0698e20510d47f5027a414ffeebaefa92285cc84fa030f4f70656e5468726561642d653439630102e49c0410b92f8c7fbb4f9f3e08492ee3915fbd2f0c0402a0fff8
    Done
    ```
5.  **Determine Matter device’s discriminator and setup PIN code**: This device
    **discriminator** is `3840` and **PIN code** is `20202021` (default)
6.  **Commission Matter device into existing IP network**: Commissioning into
    Thread network over Bluetooth LE: To commission the device to the existing
    Thread network, use the following command pattern:
    ```
    $ chip-tool pairing ble-thread <node_id> hex:<operational_dataset> <pin_code> <discriminator>
    ```
    > <node_id> is the user-defined ID of the node being commissioned.
    > <operational_dataset> is the Operational Dataset determined in the step 4.
    > <pin_code> and <discriminator are device-specific keys determined in the
    > step 5.
7.  **Control application Data Model clusters** Use the following command
    pattern to toggle the Matter Light state:
    ```
    $ chip-tool onoff toggle <node_id> <endpoint_id>
    ```
    > <node_id> is the user-defined ID of the commissioned node. <endpoint_id>
    > is the ID of the endpoint with OnOff cluster implemented.
