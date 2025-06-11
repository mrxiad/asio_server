# ASIO Chat Server

This project is a simple chat server built with **Boost.Asio**. It provides a C++
backend that communicates with a small JavaScript front end via WebSockets.

For configuration details, see [docs/CONFIGURATION.md](docs/CONFIGURATION.md).

## Key Dependencies

- Boost (development headers)
- MySQL client libraries
- CMake and a C++17 compiler
- Node.js (for the optional front-end server)

## Building the Server

```bash
cd server
mkdir build && cd build
cmake ..
make
```

The resulting `server` executable will be placed in the `build` directory.

## Running

Ensure the MySQL service is running and then start the server:

```bash
./server
```

If you wish to use the front-end client, start the WebSocket bridge:

```bash
node ../../front/websocket-server.js
```

## Installing Boost on Ubuntu

1. 执行命令

```bash
sudo apt-get update
sudo apt-get install libboost-all-dev
```
