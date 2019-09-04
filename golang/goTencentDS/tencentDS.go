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

	//FIXME: use the productURL
	tencentURL = developURL

	getToken       = "/map_ugc/get_token"
	registerDevice = "/map_ugc/register_device"
	trackReport    = "/map_ugc/track_report"
	commintOrder   = "/map_ugc/commit_order"
	flowReport     = "/map_ugc/flow_report"

	contentType = "application/x-www-form-urlencoded;charset=utf-8"

	//below two const from Tencent engineer
	tencentchannel = "3_9"
	tencentAPPID   = "device_pufangda_cloud"

	//FIXME: need info from tencent
	tencentAK        = ""
	tencentServiceID = 10
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

	resp, err := sendHTTPSRequest(tencentURL+getToken, data)
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

	req := &RegisterThirdPartyReq{
		DeviceId: &info.DeviceID,
		Channel:  &channel,
		Ts:       &t,
		Checker:  &checker,
		Imei:     &info.Imei,
	}

	data, err := proto.Marshal(req)
	if err != nil {
		glog.Errorf("RegisterDevice: Marshal error %v", err)
		return "", 0, err
	}

	resp, err := sendHTTPSRequest(tencentURL+registerDevice, data)
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

// TrackReport ...
func (c *TencentDataServer) TrackReport(trackdata *TrackData, deviceID string) (int32, error) {
	ak := tencentAK
	var pbversion int32 = 1
	var si int32 = tencentServiceID

	var req ThirdTrackReport
	req.Ak = &ak
	req.ServiceId = &si
	req.FacilityId = &deviceID
	req.Tracks = trackdata.Tracks
	req.StartTime = trackdata.StartTime
	req.EndTime = trackdata.EndTime
	req.PbVersion = &pbversion

	pb, err := proto.Marshal(&req)
	if err != nil {
		glog.Errorf("TrackReport: Marshal error %v", err)
		return 0, err
	}

	resp, err := sendHTTPSRequest(tencentURL+trackReport, pb)
	if err != nil {
		glog.Errorf("TrackReport: sendHTTPSRequest error %v", err)
		return 0, err
	}

	rsp := &ThirdTrackResp{}
	err = proto.Unmarshal(resp, rsp)
	if err != nil {
		glog.Errorf("TrackReport: unmarshaling error %v resp %v\n", err, resp)
		return 0, err
	}

	glog.Infof("TrackReport: errno %v msg %v serviceID %v deviceID %v\n",
		rsp.GetErrno(), rsp.GetMsg(), rsp.GetServiceId(), rsp.GetFacilityId())
	return rsp.GetErrno(), nil
}

// CommitOrder ...
func (c *TencentDataServer) CommitOrder(deviceID string, version string, orders []*OrderCommit_Order) (int32, error) {
	req := &OrderCommit{
		DeviceId: &deviceID,
		Version:  &version,
		Orders:   orders,
	}

	data, err := proto.Marshal(req)
	if err != nil {
		glog.Errorf("CommitOrder: Marshal error %v", err)
		return 0, err
	}

	resp, err := sendHTTPSRequest(tencentURL+flowReport, data)
	if err != nil {
		glog.Errorf("CommitOrder: sendHTTPSRequest error %v", err)
		return 0, err
	}

	rsp := &OrderCommitResp_OrderResp{}
	err = proto.Unmarshal(resp, rsp)
	if err != nil {
		glog.Errorf("CommitOrder: unmarshaling error %v resp %v\n", err, resp)
		return 0, err
	}

	glog.Infof("CommitOrder: status %v msg %v orderid %v\n",
		rsp.GetCommitStatus(), rsp.GetMsg(), rsp.GetOrderId())
	return rsp.GetCommitStatus(), nil
}

// FlowReport ...
func (c *TencentDataServer) FlowReport(flows []*ThirdFlowReportReq_DeviceFlow) (int32, error) {
	req := &ThirdFlowReportReq{
		DeviceFlows: flows,
	}

	data, err := proto.Marshal(req)
	if err != nil {
		glog.Errorf("FlowReport: Marshal error %v", err)
		return 0, err
	}

	resp, err := sendHTTPSRequest(tencentURL+flowReport, data)
	if err != nil {
		glog.Errorf("FlowReport: sendHTTPSRequest error %v", err)
		return 0, err
	}

	rsp := &ThirdFlowReportRsp{}
	err = proto.Unmarshal(resp, rsp)
	if err != nil {
		glog.Errorf("FlowReport: unmarshaling error %v resp %v\n", err, resp)
		return 0, err
	}

	glog.Infof("FlowReport: code %v msg %v\n",
		rsp.GetCode(), rsp.GetMsg())
	return rsp.GetCode(), nil
}
