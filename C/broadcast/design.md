# 目的
实现C/C++版本的本地广播，进程间通信

# 实现方式
基于tcp/socket + protocol buffer实现
* 一个服务进程
* 其他进程connect到服务进程

# 细节
1 server进程启动，监听在某个固定的端口(编译阶段决定)
2 client进程启动，连接到server进程，后续命令通过socket发送
3 client进程发送约定格式的REGISTER命令，表明自己注册哪些广播
4 client有广播变化，再次发送REGISTER命令即可
5 client发送广播，发送BROADCAST命令
6 server收到client广播，遍历所有连接的client，将消息转发给REGISTER的socket
7 其他client收到广播

# 数据结构
{
    uint32_t syncwords;     //同步字    0xA8C51D9B
    uint16_t type;          //消息类型
    uint16_t size;          //消息长度 max 65536, 消息足够的长度
    uint8_t pb[size];       //pb格式的数据，长度由size决定，消息类型由type决定 
}

# 消息类型
enum type {
    BROADCAST_REGISTER = 0,
    BROADCAST_SEND = 1,
}

# 注册广播BROADCAST_REGISTER

message BroadCastReg {
    string broadcast = 1; //广播类型
}

# 发送广播BROADCAST_SEND

message BroadCastSend {
    string broadcast = 1; //广播类型
}