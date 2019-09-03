// Code generated by protoc-gen-go. DO NOT EDIT.
// source: cs.proto

package main

import (
	fmt "fmt"
	proto "github.com/golang/protobuf/proto"
	math "math"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion3 // please upgrade the proto package

type MessageType int32

const (
	MessageType_GPSMESSAGE MessageType = 0
	MessageType_PICMESSAGE MessageType = 1
)

var MessageType_name = map[int32]string{
	0: "GPSMESSAGE",
	1: "PICMESSAGE",
}

var MessageType_value = map[string]int32{
	"GPSMESSAGE": 0,
	"PICMESSAGE": 1,
}

func (x MessageType) String() string {
	return proto.EnumName(MessageType_name, int32(x))
}

func (MessageType) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_af7bf51985781725, []int{0}
}

type DeviceGPSMessage struct {
	Deviceid             int32    `protobuf:"varint,1,opt,name=deviceid,proto3" json:"deviceid,omitempty"`
	Gpsdata              int32    `protobuf:"varint,2,opt,name=gpsdata,proto3" json:"gpsdata,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *DeviceGPSMessage) Reset()         { *m = DeviceGPSMessage{} }
func (m *DeviceGPSMessage) String() string { return proto.CompactTextString(m) }
func (*DeviceGPSMessage) ProtoMessage()    {}
func (*DeviceGPSMessage) Descriptor() ([]byte, []int) {
	return fileDescriptor_af7bf51985781725, []int{0}
}

func (m *DeviceGPSMessage) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_DeviceGPSMessage.Unmarshal(m, b)
}
func (m *DeviceGPSMessage) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_DeviceGPSMessage.Marshal(b, m, deterministic)
}
func (m *DeviceGPSMessage) XXX_Merge(src proto.Message) {
	xxx_messageInfo_DeviceGPSMessage.Merge(m, src)
}
func (m *DeviceGPSMessage) XXX_Size() int {
	return xxx_messageInfo_DeviceGPSMessage.Size(m)
}
func (m *DeviceGPSMessage) XXX_DiscardUnknown() {
	xxx_messageInfo_DeviceGPSMessage.DiscardUnknown(m)
}

var xxx_messageInfo_DeviceGPSMessage proto.InternalMessageInfo

func (m *DeviceGPSMessage) GetDeviceid() int32 {
	if m != nil {
		return m.Deviceid
	}
	return 0
}

func (m *DeviceGPSMessage) GetGpsdata() int32 {
	if m != nil {
		return m.Gpsdata
	}
	return 0
}

func init() {
	proto.RegisterEnum("main.MessageType", MessageType_name, MessageType_value)
	proto.RegisterType((*DeviceGPSMessage)(nil), "main.DeviceGPSMessage")
}

func init() { proto.RegisterFile("cs.proto", fileDescriptor_af7bf51985781725) }

var fileDescriptor_af7bf51985781725 = []byte{
	// 131 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xe2, 0xe2, 0x48, 0x2e, 0xd6, 0x2b,
	0x28, 0xca, 0x2f, 0xc9, 0x17, 0x62, 0xc9, 0x4d, 0xcc, 0xcc, 0x53, 0xf2, 0xe0, 0x12, 0x70, 0x49,
	0x2d, 0xcb, 0x4c, 0x4e, 0x75, 0x0f, 0x08, 0xf6, 0x4d, 0x2d, 0x2e, 0x4e, 0x4c, 0x4f, 0x15, 0x92,
	0xe2, 0xe2, 0x48, 0x01, 0x8b, 0x65, 0xa6, 0x48, 0x30, 0x2a, 0x30, 0x6a, 0xb0, 0x06, 0xc1, 0xf9,
	0x42, 0x12, 0x5c, 0xec, 0xe9, 0x05, 0xc5, 0x29, 0x89, 0x25, 0x89, 0x12, 0x4c, 0x60, 0x29, 0x18,
	0x57, 0x4b, 0x97, 0x8b, 0x1b, 0x6a, 0x40, 0x48, 0x65, 0x41, 0xaa, 0x10, 0x1f, 0x17, 0x17, 0xc8,
	0x48, 0xd7, 0xe0, 0x60, 0x47, 0x77, 0x57, 0x01, 0x06, 0x10, 0x3f, 0xc0, 0xd3, 0x19, 0xc6, 0x67,
	0x4c, 0x62, 0x03, 0xbb, 0xc2, 0x18, 0x10, 0x00, 0x00, 0xff, 0xff, 0x9f, 0x53, 0x27, 0x75, 0x91,
	0x00, 0x00, 0x00,
}
