package main

import (
	"bytes"
	"fmt"
)

type Msg struct {
	// header
	Len uint32 //实际上是 Flag （1个字节，通常是0） + 3个字节的长度
	Cmd uint16
	Seq uint16

	// body
	Extra []byte //avaliable only when Len's 高8位==0xEA，前面4个字节是Extra的长度
	Body  []byte
}

func main() {
	// var msg *Msg = &Msg{}

	var msg1 *Msg = &Msg{}
	fmt.Printf("len : %v\n", len(msg1.Body))

	buffer := new(bytes.Buffer)
	buffer.Write(msg1.Body)

	fmt.Printf("buffer size %v\n", len(buffer.Bytes()))
}