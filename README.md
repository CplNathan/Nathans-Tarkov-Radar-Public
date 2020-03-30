# Nathans-Tarkov-Radar
A collection of applications that make up my tarkov radar.

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
The interface client is based in Javascript and displays information in a top-down radar fashion.
The idea was to host this on a web-server allowing friends to easily view it if they wanted.

It is quite primative and runs rather poorly however it and is an example of how you could intergrate your own solution.

![Radar Image](https://i.imgur.com/mnl79g1.png)

## Notice
I really wouldn't use this for production or expose the web socket server over the internet. I attempted to implement a rubish authentication method that works if your goal is to share it between friends, however the host does not have authentication and is insecure.

#### VMRead
Thanks to Heep042 for maintaining VMRead, without it this program would not exist.

Extra Notice:
If you are trying to compile/inject the client internally it will likely hang due to my usage of sleep so please use external only.
