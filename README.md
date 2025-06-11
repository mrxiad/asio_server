# 本项目是基于asio搭建的网络聊天室

## ubuntu下载boost库

1. 执行命令	

```bash
sudo apt-get update
sudo apt-get install libboost-all-dev
```

## 构建与运行

1. 安装依赖

   ```bash
   sudo apt-get install build-essential cmake libmysqlclient-dev
   ```

2. 编译服务器

   ```bash
   cd server
   mkdir build && cd build
   cmake ..
   make
   ```

3. 运行

   ```bash
   ./server
   ```
