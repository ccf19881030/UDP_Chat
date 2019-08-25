# UDP_Chat
## 一、基于UDP的Linux C++聊天室程序
### [基于UDP的简单聊天室](https://blog.csdn.net/qq_34938530/article/details/79946692)

## 二、聊天室功能：     
###  1.其他用户上线通知。    
###  2.其他用户下线通知。
###  3.获取在线列表。
###  4.用户之间点对点聊天。

## 三、实现思想：
### 1.服务器端主要是解析客户端发送过来的各种指令，并作出相应的处理和回应。
### 2.客户端采用select管理套接口IO和标准输入IO，当有事件发生，做出相应的处理。
### 3.采用链表存储每个客户端的网络信息，登录对应链表插入（使用头插法），退出对应链表删除。点对点对应链表的遍历查找。

## 在Linux下编译和运行
### 任选一个Linux操作系统，本人在Windows10下安装的VMware15虚拟机中安装了CentOS7.6和Ubuntu18.10桌面版做过测试。
打开终端，在shell下执行如下命令，编译生成server和client二进制程序：
```shell
make
```
### 运行服务端程序：
```shell
[root@localhost UDP_Chat]# ls
client.cpp  makefile  pub.h  README.md  server.cpp
[root@localhost UDP_Chat]# make
g++ -o server server.cpp -Wall -g
g++  -o client client.cpp -Wall -g
[root@localhost UDP_Chat]# ls
client  client.cpp  makefile  pub.h  README.md  server  server.cpp
[root@localhost UDP_Chat]# ./server
has a user login:root <-> 127.0.0.1:46627
sending user list information to:root <-> 127.0.0.1:46627

```

### 运行客户端程序：
```shell
[root@localhost UDP_Chat]# ./client
please input your name:root
user root already logined server
has 1 users logined server
0 root <-> 127.0.0.1:46627
**********************************COMMANDS:*******************************
 			send username msg
			list
			exit
```
