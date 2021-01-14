# Nathans-Tarkov-Radar
A collection of applications that make up my Tarkov radar.

## Features
- Memory Reading Client (DMA, /Source/)
- Relay Server (/wwwCheats/TarkovRadarProcessor/)
- Interface Client (/wwwCheats/www/)

## Model
#### Linux Client
The Linux host client reads memory from the game running in a virtual machine using Vmread.
(https://github.com/Heep042/vmread)
The client can easily by updated to support PCILeech by templating out the read functions.
Offsets need to be updated, find the LGW offset, and mono-dissect using cheat engine to find everything you need.

#### Relay Server
The relay server is a proxy used to transmit the game data from the Linux client to supporting interfaces, custom interfaces can be made to take advantage of the relay server.

#### Interface Client
The interface is a web-based JavaScript client that renders the information received from the relay server, the provided interface renders a top-down radar and supports loot filtering.
The idea being to share access to friends using only a web browser, minimizing detection vectors.

**Works only with Chrome or Firefox with special features enabled, see: [OffscreenCanvas Compatibility](https://developer.mozilla.org/en-US/docs/Web/API/OffscreenCanvas#Browser_compatibility)**

![Radar Image](https://i.imgur.com/mnl79g1.png)

## Notice
I attempted to add some basic authentication using a token but really it is probably trivial to crack and does not provide any real security.

#### VMRead
The Linux client relies on Vmread, thank you Heep042 for making such an easy-to-use library.

Extra Notice:
Vmread supports internal and external operation, however the client only works externally.

For examples on how to setup the environment please check out the VMRead thread on unknowncheats or look at the enclosing readme.
