# Nathans-Tarkov-Radar
A collection of applications that make up my Tarkov radar.

## Features
- Memory Reading Client (DMA, /Source/)
- Relay Server (/wwwCheats/TarkovRadarProcessor/)
- Interface Client (/wwwCheats/www/)

## Model
#### Client
The client handles all the logic for reading the data from the game thanks to VMRead.
(https://github.com/Heep042/vmread)

#### Relay Server
The relay server takes the input from the program running on the host machine and relays it to other authenticated clients.

It is compatible with any program that can interface with it, meaning you can design **your own rendering implementation** to display the results in an overlay.

#### Interface Client
The interface client is based in JavaScript and displays information in a top-down radar fashion.
The idea was to host this on a web-server allowing friends to easily view it if they wanted.

**Works only with Chrome or Firefox with special features enabled, see: [OffscreenCanvas Compatibility](https://developer.mozilla.org/en-US/docs/Web/API/OffscreenCanvas#Browser_compatibility)**

It is quite primitive and runs rather poorly however it is an example of how you could integrate your own solution.

![Radar Image](https://i.imgur.com/mnl79g1.png)

## Notice
I really wouldn't use this for production or expose the web socket server over the internet. I attempted to implement a rubbish authentication method that works if your goal is to share it between friends.
However, keep in mind the host does not have authentication and is insecure.

#### VMRead
Thanks to Heep042 for maintaining VMRead, without it this program would not exist.

Extra Notice:
If you are trying to compile/inject the client internally it will likely hang due to my usage of sleep so, please use external only.

For examples on how to setup the environment please check out the VMRead thread on unknown cheats.
