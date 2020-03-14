# ddos2
ddos2 – utility for denial of service attacks. **WORK IN PROGRESS**
# Building
Currently debug builds are supported only on the systems with `gcc-9` present.<br>
To build program in release mode:<br>

`./build.sh all`
Note that for this build command `gcc` is required.  

To build program in debug mode:<br>
`./build.sh all-debug`

Note that for this build command `gcc-9` and libraries `libasan`, `libubsan` are required.  

# Modules
ddos2 uses modules to implement network interfaces and perform different scenarios of attack. Modules are stored in format of shared objects(`.so`). Each module should have functions that called when it is loaded, when optioons parsed and finally when program was initiallized. More documentation will soon appear in wiki. Modules that are expected to be in release of version 2.0:
* `mod_tcp` – implements tcp interface.
* `mod_udp` – implements udp interface.
* `mod_icmp` – implemnts icmp interface.
* `tcp` – simple DoS attacks over TCP.
* `udp` – DoS attacks over UDP.
* `icmp` – icmp DoS attack
# Network interfaces
As already was said module can implement network interface. The idea of any network interface it to send or receive packets to some target. So it is enough to implement functions that send/receive packets. For some of them it is required to open connections, so there is more functions in interface(`iface_t` in `network.h`) that helps to work with connections if this is required by protocol.

