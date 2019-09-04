package main

import (
	"bytes"
	"crypto/md5"
	"crypto/tls"
	"encoding/hex"
	fmt "fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/golang/glog"

	proto "github.com/golang/protobuf/proto"
)

const (
	developURL = "https://thirdpartyaccess.sparta.html5.qq.com"
	productURL = "https://ugcaccess.map.qq.com"

	getToken       = "/map_ugc/get_token"
	registerDevice = "/map_ugc/register_device"

	contentType = "application/x-www-form-urlencoded;charset=utf-8"

	//below two const from Tencent engineer
	tencentchannel = "3_9"
	tencentAPPID   = "device_pufangda_cloud"
)

// TencentDSCommonRsp ...
type TencentDSCommonRsp struct {
	code int
	msg  string
}

// TencentDataServer ...
type TencentDataServer struct {
}

func md5sum(in string) string {
	md5Ctx := md5.New()
	md5Ctx.Write([]byte(in))
	cipherBytes := md5Ctx.Sum(nil)
	return hex.EncodeToString(cipherBytes)
}

//FIXME: use keepalive connection
func sendHTTPSRequest(url string, data []byte) ([]byte, error) {
	tr := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}
	client := &http.Client{Transport: tr}

	resp, err := client.Post(url, contentType, bytes.NewReader(data))
	if err != nil {
		glog.Errorf("client.Post error: %v", err)
		return nil, err
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)

	return body, err
}

// GetToken ...
func (c *TencentDataServer) GetToken(deviceID string) (string, int32, error) {
	channel := tencentchannel
	t := uint64(time.Now().UnixNano() / 1000000)

	// app_secret string Y app_id + device_id + channel + ts 依次拼接为字符串，计算其 md5
	var sc = tencentAPPID
	sc += deviceID
	sc += tencentchannel
	sc += fmt.Sprintf("%d", t)
	m := md5sum(sc)
	glog.Infof("GetToken: sc = %v => md5 %v\n", sc, m)

	var req GetTokenReq
	req.DeviceId = &deviceID
	req.Channel = &channel
	req.Ts = &t
	req.AppSecret = &m

	data, err := proto.Marshal(&req)
	if err != nil {
		glog.Errorf("GetToken: ProtocolBuffer Marshal failed %v", err)
		return "", 0, err
	}

	//FIXME: use the productURL
	resp, err := sendHTTPSRequest(developURL+getToken, data)
	if err != nil {
		glog.Errorf("GetToken: sendHTTPSRequest failed %v", err)
		return "", 0, err
	}

	rsp := &GetTokenRsp{}
	err = proto.Unmarshal(resp, rsp)
	if err != nil {
		glog.Errorf("GetToken: unmarshaling error: %v resp %v\n", err, resp)
		return "", 0, err
	}

	glog.Infof("GetToken: code %v msg %v token %v expired_time %v\n", rsp.GetCode(), rsp.GetMsg(), rsp.GetToken(), rsp.GetExpiredTime())
	return rsp.GetToken(), rsp.GetCode(), nil
}

// RegisterDevice ...
func (c *TencentDataServer) RegisterDevice(info *DeviceInfo, token string) (string, int32, error) {
	channel := tencentchannel
	t := uint64(time.Now().UnixNano() / 1000000)
	sc := token
	sc += fmt.Sprintf("%d", t)
	checker := md5sum(sc)

	var req RegisterThirdPartyReq
	req.DeviceId = &info.DeviceID
	req.Channel = &channel
	req.Ts = &t
	req.Checker = &checker
	req.Imei = &info.Imei

	data, err := proto.Marshal(&req)
	if err != nil {
		glog.Errorf("RegisterDevice: Marshal error %v", err)
		return "", 0, err
	}

	//FIXME: use the productURL
	resp, err := sendHTTPSRequest(developURL+registerDevice, data)
	if err != nil {
		glog.Errorf("RegisterDevice: sendHTTPSRequest error %v", err)
		return "", 0, err
	}

	rsp := &RegisterThirdPartyRsp{}
	err = proto.Unmarshal(resp, rsp)
	if err != nil {
		glog.Errorf("RegisterDevice: unmarshaling error %v resp %v\n", err, resp)
		return "", 0, err
	}

	glog.Infof("RegisterDevice: code %v msg %v deviceID %v\n",
		rsp.GetCode(), rsp.GetMsg(), rsp.GetDeviceId())
	return rsp.GetDeviceId(), rsp.GetCode(), nil
}
