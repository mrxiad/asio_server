# 本项目是基于asio搭建的网络聊天室

## ubuntu下载boost库

1. 执行命令

        ```bash
        sudo apt-get update
        sudo apt-get install libboost-all-dev
        ```

## 构建依赖库

本仓库不再包含生成后的 `so` 库，需要在本地构建。

1. 安装依赖：

        ```bash
        sudo apt-get update
        sudo apt-get install build-essential libmysqlclient-dev libjsoncpp-dev
        ```

2. 执行脚本编译库文件：

        ```bash
        bash scripts/build_libs.sh
        ```

生成的库会位于 `output/lib` 目录中，CMake 会在该目录中查找 `lib_public.so`、`lib_mysql.so` 以及系统提供的 `libjsoncpp.so`。
