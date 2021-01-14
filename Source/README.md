# Nathans-Tarkov-Radar
Linux Client

## Building
This project uses the meson build system, to build it run `meson` while specifying the build directory as a parameter.
`meson build`
In the build directory run `ninja` to compile, an executable named `TarkovInjector` will be produced.

## Running
To execute the project run it as root providing the WebSocket URL and passcode as two separate parameters, you can provide the optional parameter -s to auto reconnect to the WebSocket on disconnection.
`sudo ./TarkovInjector http://127.0.0.1:8080 boop -s`
`sudo ./TarkovInjector --help`
