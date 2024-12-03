---

# NXP OpenThread Border Router

A Thread Border Router connects a Thread network to other IP-based networks, such as Wi-Fi or Ethernet. A Thread network requires a Border Router to connect to other networks.

A Thread Border Router minimally supports the following functions:

- End-to-end IP connectivity via routing between Thread devices and other external IP networks
- External Thread Commissioning (for example, a mobile phone) to authenticate and join a Thread device to a Thread network

NXP OTBR includes a number of features, including:

- Service registration : Nodes on the Thread network can register services to the border router. Border router itself registers its own services.
- Service discovery between LAN and Thread network:
  - Advertising proxy : The nodes on the backbone link can find services on the Thread Network.
  - Discovery proxy : The nodes on the Thread network can find mDNS services on the backbone link.
- Border Routing capability.
- mDNS Publisher - Allows an External commissioner to discover a Border Router and its associated Thread network.

## Getting started

To learn more about building and running the examples please check:

- [OpenThread on RT1170 examples][rt1170-page]
- [OpenThread on RT1060 examples][rt1060-page]
- [OpenThread on RW612 examples][rw612-page]

[rt1170-page]: ../../src/imx_rt/rt1170/README.md
[rt1060-page]: ../../src/imx_rt/rt1060/README.md
[rw612-page]: ../..src/rw/rw612/README.md

Current border router example supports wifi or ethernet. The default is WIFI. To use ethernet, add the following options to the build command:

```bash
$ ./script/build_rw612 -DOT_NXP_LWIP_ETH=ON -DOT_NXP_LWIP_WIFI=OFF
```

## How to run the OTBR application

In order to run the OTBR example, a minimum of devices is required: - NXP OTBR - A Thread CLI capable device - A windows/linux PC to run an mDNS client.

Note: For this demonstration, Ubuntu 21.10 with mDNSResponder was tested. Avahi can also be used, but it has some limitations of the mDNS standard.

It is important that the Border Router and the PC to reside on the same Wi-Fi network

To connect to the wifi network we can use the follwing cli command:

```
> wifi connect <wifi_ssid> <wifi_passwd>
done
```

To see the IP addresses assigned to each interface use:

```
> lwip ipaddr
en2:
 #0: FE80::D4D1:71FF:FEC8:6B42
      preferred static
 #1: FD37:C34C:FBD3:FD30:D4D1:71FF:FEC8:6B42
      preferred pref_lft: forever valid_lft: forever

 IPv4 Address: 192.168.2.207/24
 Gateway     : 192.168.2.1

ot1:
 #0: FE80::8E0:2E65:6468:39C1
      deprecated static
 #1: FDB9:BDB1:412E:A443:4141:1B14:CE94:9111
      deprecated static
 #2: FDB9:BDB1:412E:A443:0:FF:FE00:FC00
      deprecated static
 #3: FDB9:BDB1:412E:A443:0:FF:FE00:FC10
      deprecated static
 #4: FDB9:BDB1:412E:A443:0:FF:FE00:FC38
      deprecated static
 #5: FD37:C34C:FBD3:FD34:E0D:BCDF:6294:40D6
      preferred static
 #6: FDB9:BDB1:412E:A443:0:FF:FE00:FC11
      deprecated static

lo0:
 #0: ::1
      deprecated static

 IPv4 Address: 127.0.0.1/8
 Gateway     : 127.0.0.1

Done
```

To start the Thread network see the steps from this [readme][rw612-page]:

[rw612-page]: ../..src/rw/rw612/README.md

### Advertising proxy functionality:

Publish a SRP service from the Thread device. One example could be:

[SRP registration example][openthread cli - srp (service registration protocol)]

[openthread cli - srp (service registration protocol)]: https://github.com/openthread/openthread/blob/main/src/cli/README_SRP.md

After registering, the service can be seen in SRP list, by issuing `srp server service` command on Border Router's CLI.

```
> srp server service
my-service._ipps._tcp.default.service.arpa.
    deleted: false
    subtypes: (null)
    port: 12345
    priority: 0
    weight: 0
    ttl: 7200
    lease: 7200
    key-lease: 680400
    TXT: []
    host: my-host.default.service.arpa.
    addresses: [fd99:d31e:b15d:458c:4c3e:d565:1c50:57ea]
```

This service can also be discovered from LAN using the mDNS client:
`dns-sd -B _ipps._tcp`

The console output is shown below:

```
ubuntu@ubuntu:~$ dns-sd -B _ipps._tcp
Browsing for _ipps._tcp
DATE: ---Fri 12 May 2023---
 8:56:41.473  ...STARTING...
Timestamp     A/R    Flags  if Domain               Service Type         Instance Name
 8:56:41.883  Add        2   3 local.               _ipps._tcp.          my-service
```

Information about the host of this service can be found by executing this command: `dns-sd -L my-service _ipps._tcp local`.

The console output is shown below:

```
ubuntu@ubuntu:~$ dns-sd -L my-service _ipps._tcp local
Lookup my-service._ipps._tcp.local
DATE: ---Fri 12 May 2023---
 8:59:24.350  ...STARTING...
 8:59:24.351  my-service._ipps._tcp.local. can be reached at my-host.local.:12345 (interface 3)
```

Obtaining a host's IPV6 address/addresses can be done by executing this command: `dns-sd -G v6 my-host.local`.

The console output is shown below:

```
ubuntu@ubuntu:~$ dns-sd -G v6 my-host.local
DATE: ---Fri 12 May 2023---
 9:00:53.039  ...STARTING...
Timestamp     A/R  Flags         IF  Hostname                               Address                                      TTL
 9:00:53.040  Add  40000002       3  my-host.local.                         FD99:D31E:B15D:458C:4C3E:D565:1C50:57EA%<0>  4500
```

### Discovery proxy functionality :

Using mDNSResponder, publish the service:
`dns-sd -R WifiTestService _http._tcp local 80`

The command result is shown below:

```
Registering Service WifiTestService._http._tcp.local port 80
DATE: ---Fri 12 May 2023---
6:51:25.617  ...STARTING...
6:51:26.352  Got a reply for service WifiTestService._http._tcp.local.: Name now registered and active
```

Obtain Border Router's MLEID ip address, `ipaddr mleid` and then on the Thread device execute `dns config <IPaddr>`

Obtaining information about `WifiTestService` can be done by executing this command on the Thread device:

```
dns service WifiTestService _http._tcp.default.service.arpa.
```

The console output on the Thread device is shown below:

```
DNS service resolution response for WifiTestService for service _http._tcp.default.service.arpa.
Port:80, Priority:0, Weight:0, TTL:120
Host:ubuntu.default.service.arpa.
HostAddress:2a02:2f01:7b11:6a00:e65f:1ff:fef5:ff11 TTL:120
TXT:[] TTL:120
```

A browse resolution query can be send executing the following command:
`dns browse _http._tcp.default.service.arpa.`

This command will return all instances with the "\_http.\_tcp" service type:

```
DNS browse response for _http._tcp.default.service.arpa.
WifiTestService
    Port:80, Priority:0, Weight:0, TTL:120
    Host:ubuntu.default.service.arpa.
    HostAddress:2a02:2f01:7b11:6a00:e65f:1ff:fef5:ff11 TTL:120
    TXT:[] TTL:120
```

To obtain the IPV6 address of a host name, an AAAA query is sent by executing the following command:
`dns resolve ubuntu.default.service.arpa.`

The console output on the Thread device is shown below:

```
DNS response for ubuntu.default.service.arpa. - 2a02:2f01:7b11:6a00:e65f:1ff:fef5:ff11 TTL:120 fdde:ad00:beef:cafe:e65f:1ff:fef5:ff11 TTL:120
```
