package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"net"
	"os"
	"os/exec"
	"path/filepath"
	"time"

	proto "github.com/golang/protobuf/proto"
)

/*

# TCPMsg 核心协议

数据都使用普通的TCP协议，由于TCP是流式没有边界，因此统一规定每个TCP的消息的格式为：

======================================================================
| Len  | 4字节，表示CMD+Seq+Data的长度（不包括Len本身）
----------------------------------------------------------------------
| Cmd  | 2个字节，表示某个命令，uint16_t.
-----------------------------------------------------------------------
| Seq  | 2个字节，表示SequenceID，发送response时，方便和Request的一一对应。
-----------------------------------------------------------------------
| Data | 通常是采用PB（Protocol Buffer）格式的数据。或者前面4个字节是设备ID（uint32）（用于FS到PS的通讯）
-----------------------------------------------------------------------
*/
type messsageHeader struct {
	Len int32
	Cmd int16
	Seq int16
}

const (
	serverURL = "127.0.0.1:12345"
)

func printBanner() {
	file, _ := exec.LookPath(os.Args[0])
	path, _ := filepath.Abs(file)
	path = filepath.Dir(path)

	t := time.Now()
	fmt.Println("Program start at ", path, ",time=", t)
}

func fillPBData() *bytes.Buffer {
	var gpsData DeviceGPSMessage
	gpsData.Deviceid = 12345678
	gpsData.Gpsdata = 22123456
	data, _ := proto.Marshal(&gpsData)
	fmt.Printf("fillPBData -> Deviceid %v Gpsdata %v\n", gpsData.Deviceid, gpsData.Gpsdata)

	var header messsageHeader
	header.Cmd = int16(MessageType_GPSMESSAGE)
	header.Seq = 1
	header.Len = int32(binary.Size(header) - binary.Size(header.Len) + len(data))
	fmt.Printf("fillPBData -> Cmd %v Seq %v Len %v\n", header.Cmd, header.Seq, header.Len)
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, &header)
	buf.Write(data) //将切片d写入Buffer尾部

	return buf
}

func parsePBData(data []byte) {
	gpsData := &DeviceGPSMessage{}
	err := proto.Unmarshal(data, gpsData)
	if err != nil {
		fmt.Printf("unmarshaling error: %v\n", err)
		return
	}

	fmt.Printf("parsePBData <- Deviceid %v Gpsdata %v\n", gpsData.GetDeviceid(), gpsData.GetGpsdata())
}

func onNewConn(conn net.Conn) {
	fmt.Printf("got new tcp client connection: %v, remote addr: %v\n", conn, conn.RemoteAddr().String())
	defer func() {
		conn.Close()
	}()

	var stage = 0 // 0: read header 1: read body
	var readLen = 0
	var header messsageHeader
	reader := bufio.NewReader(conn)
	for {
		if stage == 0 {
			readLen = binary.Size(header)
		} else {
			readLen = int(header.Len) - (binary.Size(header) - binary.Size(header.Len)) //len include some size of header
		}

		data := make([]byte, readLen)

		// fmt.Printf("ReadFull readLen = %v reader.Buffered() %v\n", readLen, reader.Buffered())
		_, err := io.ReadFull(reader, data)
		if err != nil {
			fmt.Println("读取客户端数据错误:", err.Error())
			break
		}

		if stage == 0 {
			binary.Read(bytes.NewReader(data), binary.LittleEndian, &header)
			fmt.Printf("parsePBData <- Cmd %v Seq %v Len %v\n", header.Cmd, header.Seq, header.Len)
			stage = 1
			continue
		}

		parsePBData(data)
		stage = 0

		conn.Write([]byte("Receive PB data from Client\n"))
	}
}

func startTCPServerForPB() {
	fmt.Printf("startTCPServerForPB Listen: %s\n", serverURL)

	listener, err := net.Listen("tcp", serverURL)
	if err != nil {
		fmt.Printf("failed to startup tcp server at %v err: %v", serverURL, err)
		return
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Printf("failed to accept tcp client at port %v error: %v", serverURL, err)
			continue
		}
		go onNewConn(conn)
	}
}

func startTCPClientForPB() {
	fmt.Printf("startTCPClientForPB: connect to %s\n", serverURL)

	conn, err := net.Dial("tcp", serverURL)
	defer func() {
		if conn != nil {
			conn.Close()
		}
	}()

	if err != nil {
		fmt.Printf("connect failed, err : %v\n", err.Error())
		return
	}
	defer conn.Close()

	buf := fillPBData()
	conn.Write(buf.Bytes())

	reader := bufio.NewReader(conn)
	msg, err := reader.ReadString('\n')
	fmt.Printf("startTCPClientForPB: %s\n", msg)

	if err != nil || err == io.EOF {
		fmt.Println(err)
		return
	}
	fmt.Println("startTCPClientForPB: exit")
}

func main() {
	printBanner()

	if len(os.Args) > 1 && os.Args[1] == "server" {
		startTCPServerForPB()
	} else {
		startTCPClientForPB()
	}
}
