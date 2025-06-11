# Configuration

This document describes how to configure and run the ASIO-based chat server.

## Server Options

The server listens on port `10086` by default. You can change the port by
editing `server/src/main.cpp` and modifying the value passed to `CServer`.

```cpp
CServer s(io_service, <PORT_NUMBER>);
```

Rebuild the project after making changes.

## Database

The example build links against MySQL. Ensure the MySQL client libraries and
headers are installed on your system. The sample SQL schema can be found in
`model/sql`.

## Front End

A simple WebSocket front end is located under `front/`. To start the websocket
bridge, run:

```bash
node front/websocket-server.js
```

This will serve the static files in `front/` and forward WebSocket connections
to the C++ server.
