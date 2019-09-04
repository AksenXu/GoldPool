package main

import (
	"github.com/golang/glog"
)

const (
	testDeviceID = "1234567890abcdef"
)

func main() {
	var ds TencentDataServer
	deviceid := testDeviceID

	token, code, err := ds.GetToken(deviceid)
	if err != nil || code != 0 {
		glog.Errorf("GetToken deviceID %v failed error %v code %v", deviceid, err, code)
		return
	}

	glog.Infof("GetToken deviceID %v token %v", deviceid, token)
	var di DeviceInfo
	di.DeviceID = testDeviceID

	_, code, err = ds.RegisterDevice(&di, token)
	if err != nil || code != 0 {
		glog.Errorf("RegisterDevice deviceID %v failed error %v code %v", deviceid, err, code)
		return
	}

	glog.Infof("RegisterDevice deviceID %v success", deviceid)
	return
}
