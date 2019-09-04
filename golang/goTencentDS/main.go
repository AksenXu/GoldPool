package main

import (
	"github.com/golang/glog"
)

const (
	testDeviceID = "1234567890abcdef"
)

var ds TencentDataServer

func testGetToken() string {
	deviceid := testDeviceID

	token, code, err := ds.GetToken(deviceid)
	if err != nil || code != 0 {
		glog.Errorf("GetToken deviceID %v failed error %v code %v", deviceid, err, code)
		return ""
	}

	glog.Infof("GetToken deviceID %v token %v", deviceid, token)
	return token
}

func testRegisterDevice(token string) {
	var di DeviceInfo
	di.DeviceID = testDeviceID

	_, code, err := ds.RegisterDevice(&di, token)
	if err != nil || code != 0 {
		glog.Errorf("RegisterDevice deviceID %v failed error %v code %v", di.DeviceID, err, code)
		return
	}

	glog.Infof("RegisterDevice deviceID %v success", di.DeviceID)
}

func initFakeGPSInfo(info *ThirdTrackInfo) {
	var gpstime int64
	var latitude float64
	var longitude float64
	var altitude float64
	var accuracy float64
	var gpsBearing float64
	var speed float64
	var locationType ThirdTrackInfo_LocationType = ThirdTrackInfo_Gps
	var locationMode ThirdTrackInfo_LocationMode = ThirdTrackInfo_D2
	var coordinateType ThirdTrackInfo_CoordinateType = ThirdTrackInfo_WGS84
	var vDop float64
	var hDop float64
	var starNum int32
	var systemTime int64
	var phoneDirection int32
	var isMockGps bool
	*info = ThirdTrackInfo{
		GpsTime:        &gpstime,
		Latitude:       &latitude,
		Longitude:      &longitude,
		Altitude:       &altitude,
		Accuracy:       &accuracy,
		GpsBearing:     &gpsBearing,
		Speed:          &speed,
		LocationType:   &locationType,
		LocationMode:   &locationMode,
		CoordinateType: &coordinateType,
		VDop:           &vDop,
		HDop:           &hDop,
		StarNum:        &starNum,
		SystemTime:     &systemTime,
		PhoneDirection: &phoneDirection,
		IsMockGps:      &isMockGps,
	}
}

func testTrackReport() {
	var startTime int32
	var endTime int32
	var info ThirdTrackInfo

	//test code, it should come from device
	initFakeGPSInfo(&info)
	tracks := make([]*ThirdTrackInfo, 1)
	tracks[0] = &info

	var trackdata TrackData
	trackdata.StartTime = &startTime
	trackdata.EndTime = &endTime
	trackdata.Tracks = tracks
	errno, err := ds.TrackReport(&trackdata, testDeviceID)
	if err != nil || errno != 0 {
		glog.Errorf("TrackReport deviceID %v failed error %v code %v", testDeviceID, err, errno)
		return
	}
}

func testCommitOrder() {

}

func testFlowReport() {
	deviceID := testDeviceID
	channel := "3_9"
	date := "20190903"
	var dataflow uint64 = 100000
	var num int32 = 1
	flow := ThirdFlowReportReq_DeviceFlow{
		DeviceId:  &deviceID,
		Channel:   &channel,
		Date:      &date,
		TrackFlow: &dataflow,
		ImgFlow:   &dataflow,
		ImgNum:    &num,
	}
	flows := make([]*ThirdFlowReportReq_DeviceFlow, 1)
	flows[0] = &flow

	errno, err := ds.FlowReport(flows)
	if err != nil || errno != 0 {
		glog.Errorf("FlowReport deviceID %v failed error %v code %v", testDeviceID, err, errno)
		return
	}
	glog.Infof("testFlowReport OK errno %v", errno)
}

func main() {

	// token := testGetToken()
	// testRegisterDevice(token)

	//need test
	testTrackReport()

	// testCommitOrder()

	// testFlowReport()
	return
}
