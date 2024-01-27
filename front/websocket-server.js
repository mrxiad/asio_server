const WebSocket = require('ws');
const net = require('net');

const wss = new WebSocket.Server({ port: 8080 });

console.log("开启中转站")
wss.on('connection', function connection(ws) {
  const tcpClient = new net.Socket();
  
  tcpClient.connect(10086, '127.0.0.1', function() {
    console.log('Connected to TCP server');
  });

  ws.on('message', function incoming(message) {
    // 确保消息是一个Buffer
    if (Buffer.isBuffer(message)) {
      tcpClient.write(message);
    }
  });

  tcpClient.on('data', function(data) {
    ws.send(data);
  });

  tcpClient.on('close', function() {
    ws.close();
  });
});
