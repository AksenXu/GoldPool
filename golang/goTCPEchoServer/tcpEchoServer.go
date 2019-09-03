package main

import (
	// "io/ioutil"
	"fmt"
	"net"
	"os"
	"os/exec"
	"path/filepath"
	"time"
)

func onNewConn(conn net.Conn) {
	fmt.Println("got new tcp client connection: %v, remote addr: %v", conn, conn.RemoteAddr().String())
	defer func() {
		conn.Close()
	}()

	data := make([]byte, 128)
	for {
		i, err := conn.Read(data)
		fmt.Println("客户端发来数据:", string(data[0:i]))
		if err != nil {
			fmt.Println("读取客户端数据错误:", err.Error())
			break
		}
		conn.Write(data[0:i])
	}

}

func RunServer() {
	listenport := ":28000"
	listener, err := net.Listen("tcp", listenport)
	if err != nil {
		fmt.Println("failed to startup tcp server at %v err: %v", listenport, err)
		return
	}

	fmt.Println("Run STcp server at %v", listenport)

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("failed to accept tcp client at port %v error: %v", listenport, err)
			continue
		}
		go onNewConn(conn)
	}
}

func main() {
	file, _ := exec.LookPath(os.Args[0])
	path, _ := filepath.Abs(file)
	path = filepath.Dir(path)

	t := time.Now()
	fmt.Println("Program start at ", path, ",time=", t)

	RunServer()
}
