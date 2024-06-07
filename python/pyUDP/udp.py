import socket
import json

data = {
    "f":"k", 
    "deviceid":"123456789012",
    "ack":0
}

json_data = json.dumps(data)

# 目标服务器地址和端口
server_address = ('8.135.14.149', 10000)

# 创建UDP套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # 发送数据
    sock.sendto(json_data.encode(), server_address)
    
    # 设置超时时间
    sock.settimeout(5)
    
    # 接收响应
    response, server = sock.recvfrom(4096)
    print(f"Received response from server: {response.decode()}")
finally:
    sock.close()

