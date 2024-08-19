# 分布式网络通信框架

## 简介

本项目是一个基于protobuf和muduo库开发的集群分布式网络通信框架。它具有良好的可扩展性和易用性，适用于构建大规模分布式系统。

## 项目结构

以下是项目的主要目录结构：

```
.
├── bin               # 可执行文件
├── build             # 项目编译文件
├── lib               # 项目库文件
├── src               # 源文件
├── test              # 测试代码
├── example           # 框架代码使用范例
├── CMakeLists.txt    # 顶层的cmake文件
├── README.md         # 项目自述文件
└── autobuild.sh      # 一键编译脚本
```

## 安装与编译

### 环境要求

- 操作系统：Linux
- 编译器：GCC 4.8及以上版本
- 其他依赖：protobuf、muduo库

### 编译步骤

1. 进入项目根目录，执行以下命令安装依赖库：

```bash
sudo apt-get install libprotobuf-dev protobuf-compiler libmuduo-dev
```

2. 进入build文件夹，按照以下步骤生成可执行文件：

```bash
cd build
cmake ..
make
```

或者直接运行以下命令：

```bash
./autobuild.sh
```

3. 成功生成后，进入bin目录，先启动服务器再启动客户端即可进行测试：

```bash
cd bin
./server
./client
```

## 测试

在test目录下，提供了相关的测试代码。您可以通过以下命令进行测试：

```bash
cd test
./unittest
```

## 使用示例

在example目录下，提供了框架代码的使用范例。您可以参考这些范例来快速上手本项目。
