// Code generated by protoc-gen-go. DO NOT EDIT.
// source: register_thirdparty.proto

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

type RegisterThirdPartyReq struct {
	DeviceId             *string  `protobuf:"bytes,1,opt,name=device_id,json=deviceId" json:"device_id,omitempty"`
	Channel              *string  `protobuf:"bytes,2,opt,name=channel" json:"channel,omitempty"`
	Ts                   *uint64  `protobuf:"varint,3,req,name=ts" json:"ts,omitempty"`
	Checker              *string  `protobuf:"bytes,4,req,name=checker" json:"checker,omitempty"`
	Imei                 *string  `protobuf:"bytes,5,opt,name=imei" json:"imei,omitempty"`
	WifiMac              *string  `protobuf:"bytes,6,opt,name=wifi_mac,json=wifiMac" json:"wifi_mac,omitempty"`
	Mac                  *string  `protobuf:"bytes,7,opt,name=mac" json:"mac,omitempty"`
	Brand                *string  `protobuf:"bytes,8,opt,name=brand" json:"brand,omitempty"`
	Manufacturer         *string  `protobuf:"bytes,9,opt,name=manufacturer" json:"manufacturer,omitempty"`
	Model                *string  `protobuf:"bytes,10,opt,name=model" json:"model,omitempty"`
	Product              *string  `protobuf:"bytes,11,opt,name=product" json:"product,omitempty"`
	Hardware             *string  `protobuf:"bytes,12,opt,name=hardware" json:"hardware,omitempty"`
	Device               *string  `protobuf:"bytes,13,opt,name=device" json:"device,omitempty"`
	OsVersion            *string  `protobuf:"bytes,14,opt,name=os_version,json=osVersion" json:"os_version,omitempty"`
	AppVersion           *string  `protobuf:"bytes,15,opt,name=app_version,json=appVersion" json:"app_version,omitempty"`
	Random               *string  `protobuf:"bytes,16,opt,name=random" json:"random,omitempty"`
	Board                *string  `protobuf:"bytes,17,opt,name=board" json:"board,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *RegisterThirdPartyReq) Reset()         { *m = RegisterThirdPartyReq{} }
func (m *RegisterThirdPartyReq) String() string { return proto.CompactTextString(m) }
func (*RegisterThirdPartyReq) ProtoMessage()    {}
func (*RegisterThirdPartyReq) Descriptor() ([]byte, []int) {
	return fileDescriptor_cf49990df58de730, []int{0}
}

func (m *RegisterThirdPartyReq) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_RegisterThirdPartyReq.Unmarshal(m, b)
}
func (m *RegisterThirdPartyReq) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_RegisterThirdPartyReq.Marshal(b, m, deterministic)
}
func (m *RegisterThirdPartyReq) XXX_Merge(src proto.Message) {
	xxx_messageInfo_RegisterThirdPartyReq.Merge(m, src)
}
func (m *RegisterThirdPartyReq) XXX_Size() int {
	return xxx_messageInfo_RegisterThirdPartyReq.Size(m)
}
func (m *RegisterThirdPartyReq) XXX_DiscardUnknown() {
	xxx_messageInfo_RegisterThirdPartyReq.DiscardUnknown(m)
}

var xxx_messageInfo_RegisterThirdPartyReq proto.InternalMessageInfo

func (m *RegisterThirdPartyReq) GetDeviceId() string {
	if m != nil && m.DeviceId != nil {
		return *m.DeviceId
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetChannel() string {
	if m != nil && m.Channel != nil {
		return *m.Channel
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetTs() uint64 {
	if m != nil && m.Ts != nil {
		return *m.Ts
	}
	return 0
}

func (m *RegisterThirdPartyReq) GetChecker() string {
	if m != nil && m.Checker != nil {
		return *m.Checker
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetImei() string {
	if m != nil && m.Imei != nil {
		return *m.Imei
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetWifiMac() string {
	if m != nil && m.WifiMac != nil {
		return *m.WifiMac
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetMac() string {
	if m != nil && m.Mac != nil {
		return *m.Mac
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetBrand() string {
	if m != nil && m.Brand != nil {
		return *m.Brand
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetManufacturer() string {
	if m != nil && m.Manufacturer != nil {
		return *m.Manufacturer
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetModel() string {
	if m != nil && m.Model != nil {
		return *m.Model
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetProduct() string {
	if m != nil && m.Product != nil {
		return *m.Product
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetHardware() string {
	if m != nil && m.Hardware != nil {
		return *m.Hardware
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetDevice() string {
	if m != nil && m.Device != nil {
		return *m.Device
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetOsVersion() string {
	if m != nil && m.OsVersion != nil {
		return *m.OsVersion
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetAppVersion() string {
	if m != nil && m.AppVersion != nil {
		return *m.AppVersion
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetRandom() string {
	if m != nil && m.Random != nil {
		return *m.Random
	}
	return ""
}

func (m *RegisterThirdPartyReq) GetBoard() string {
	if m != nil && m.Board != nil {
		return *m.Board
	}
	return ""
}

type RegisterThirdPartyRsp struct {
	Code                 *int32   `protobuf:"varint,1,req,name=code" json:"code,omitempty"`
	Msg                  *string  `protobuf:"bytes,2,req,name=msg" json:"msg,omitempty"`
	DeviceId             *string  `protobuf:"bytes,3,opt,name=device_id,json=deviceId" json:"device_id,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *RegisterThirdPartyRsp) Reset()         { *m = RegisterThirdPartyRsp{} }
func (m *RegisterThirdPartyRsp) String() string { return proto.CompactTextString(m) }
func (*RegisterThirdPartyRsp) ProtoMessage()    {}
func (*RegisterThirdPartyRsp) Descriptor() ([]byte, []int) {
	return fileDescriptor_cf49990df58de730, []int{1}
}

func (m *RegisterThirdPartyRsp) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_RegisterThirdPartyRsp.Unmarshal(m, b)
}
func (m *RegisterThirdPartyRsp) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_RegisterThirdPartyRsp.Marshal(b, m, deterministic)
}
func (m *RegisterThirdPartyRsp) XXX_Merge(src proto.Message) {
	xxx_messageInfo_RegisterThirdPartyRsp.Merge(m, src)
}
func (m *RegisterThirdPartyRsp) XXX_Size() int {
	return xxx_messageInfo_RegisterThirdPartyRsp.Size(m)
}
func (m *RegisterThirdPartyRsp) XXX_DiscardUnknown() {
	xxx_messageInfo_RegisterThirdPartyRsp.DiscardUnknown(m)
}

var xxx_messageInfo_RegisterThirdPartyRsp proto.InternalMessageInfo

func (m *RegisterThirdPartyRsp) GetCode() int32 {
	if m != nil && m.Code != nil {
		return *m.Code
	}
	return 0
}

func (m *RegisterThirdPartyRsp) GetMsg() string {
	if m != nil && m.Msg != nil {
		return *m.Msg
	}
	return ""
}

func (m *RegisterThirdPartyRsp) GetDeviceId() string {
	if m != nil && m.DeviceId != nil {
		return *m.DeviceId
	}
	return ""
}

func init() {
	proto.RegisterType((*RegisterThirdPartyReq)(nil), "main.RegisterThirdPartyReq")
	proto.RegisterType((*RegisterThirdPartyRsp)(nil), "main.RegisterThirdPartyRsp")
}

func init() { proto.RegisterFile("register_thirdparty.proto", fileDescriptor_cf49990df58de730) }

var fileDescriptor_cf49990df58de730 = []byte{
	// 349 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0x6c, 0x91, 0xc1, 0x8e, 0xda, 0x30,
	0x10, 0x86, 0x45, 0x12, 0x20, 0x19, 0x28, 0xa5, 0x56, 0x8b, 0x86, 0x56, 0x55, 0x23, 0x4e, 0x9c,
	0xfa, 0x1e, 0x3d, 0x54, 0x5a, 0x45, 0xab, 0x3d, 0xec, 0x25, 0xf2, 0xda, 0x06, 0xac, 0x25, 0xb1,
	0xd7, 0x36, 0xa0, 0x7d, 0xc0, 0x7d, 0xaf, 0x95, 0xc7, 0x61, 0x25, 0xa4, 0xbd, 0xcd, 0xf7, 0xff,
	0xbf, 0x27, 0x99, 0x19, 0x58, 0x3b, 0xb5, 0xd7, 0x3e, 0x28, 0xd7, 0x86, 0x83, 0x76, 0xd2, 0x72,
	0x17, 0x5e, 0xff, 0x5a, 0x67, 0x82, 0x61, 0x45, 0xc7, 0x75, 0xbf, 0x79, 0xcb, 0xe1, 0x47, 0x33,
	0x64, 0xee, 0x63, 0xe4, 0x2e, 0x46, 0x1a, 0xf5, 0xc2, 0x7e, 0x41, 0x25, 0xd5, 0x59, 0x0b, 0xd5,
	0x6a, 0x89, 0xa3, 0x7a, 0xb4, 0xad, 0x9a, 0x32, 0x09, 0xff, 0x24, 0x43, 0x98, 0x8a, 0x03, 0xef,
	0x7b, 0x75, 0xc4, 0x8c, 0xac, 0x2b, 0xb2, 0x05, 0x64, 0xc1, 0x63, 0x5e, 0x67, 0xdb, 0xa2, 0xc9,
	0x82, 0x4f, 0x49, 0x25, 0x9e, 0x95, 0xc3, 0xa2, 0xce, 0x52, 0x92, 0x90, 0x31, 0x28, 0x74, 0xa7,
	0x34, 0x8e, 0xa9, 0x01, 0xd5, 0x6c, 0x0d, 0xe5, 0x45, 0xef, 0x74, 0xdb, 0x71, 0x81, 0x93, 0xd4,
	0x38, 0xf2, 0x7f, 0x2e, 0xd8, 0x12, 0xf2, 0xa8, 0x4e, 0x49, 0x8d, 0x25, 0xfb, 0x0e, 0xe3, 0x27,
	0xc7, 0x7b, 0x89, 0x25, 0x69, 0x09, 0xd8, 0x06, 0xe6, 0x1d, 0xef, 0x4f, 0x3b, 0x2e, 0xc2, 0xc9,
	0x29, 0x87, 0x15, 0x99, 0x37, 0x5a, 0x7c, 0xd9, 0x19, 0xa9, 0x8e, 0x08, 0xe9, 0x25, 0x41, 0xfc,
	0x55, 0xeb, 0x8c, 0x3c, 0x89, 0x80, 0xb3, 0xf4, 0xed, 0x01, 0xd9, 0x4f, 0x28, 0x0f, 0xdc, 0xc9,
	0x0b, 0x77, 0x0a, 0xe7, 0x69, 0x15, 0x57, 0x66, 0x2b, 0x98, 0xa4, 0xb5, 0xe0, 0x17, 0x72, 0x06,
	0x62, 0xbf, 0x01, 0x8c, 0x6f, 0xcf, 0xca, 0x79, 0x6d, 0x7a, 0x5c, 0x90, 0x57, 0x19, 0xff, 0x90,
	0x04, 0xf6, 0x07, 0x66, 0xdc, 0xda, 0x0f, 0xff, 0x2b, 0xf9, 0xc0, 0xad, 0xbd, 0x06, 0x56, 0x30,
	0x89, 0xf3, 0x98, 0x0e, 0x97, 0xa9, 0x6f, 0x22, 0x9a, 0xda, 0x70, 0x27, 0xf1, 0xdb, 0x30, 0x75,
	0x84, 0xcd, 0xe3, 0xa7, 0x67, 0xf4, 0x36, 0x6e, 0x59, 0x18, 0xa9, 0x70, 0x54, 0x67, 0xdb, 0x71,
	0x43, 0x35, 0xad, 0xd2, 0xef, 0x31, 0xa3, 0x7b, 0xc4, 0xf2, 0xf6, 0xd8, 0xf9, 0xed, 0xb1, 0xdf,
	0x03, 0x00, 0x00, 0xff, 0xff, 0xb5, 0x8e, 0xdb, 0x1d, 0x45, 0x02, 0x00, 0x00,
}
