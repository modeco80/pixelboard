## PixelBoard

An infinite region-based online pixel canvas, with server software written in C++20.

## Building

### Server

#### Dependencies

 - fmt
 - spdlog
 - Boost.Asio
 - Boost.Beast

You can use vcpkg in manifest mode to install these dependencies for you, or install them yourself.

```bash
$ cmake -B build -DCMAKE_BUILD_TYPE=Release .
$ cmake --build build
# ...
# Profit?
```

Alternatively, you can use Visual Studio or CLion to build the project.

The output `pixelboard` server executable can be placed wherever you want.

### Webapp

The webapp and WebSocket functionality are not implemented yet, so this will probably change a lot.

Adjust `web/src/config.js` to fit your configuration, then run:

```bash
$ npm i
$ npm run build # TODO
```