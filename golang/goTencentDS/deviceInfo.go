package main

// DeviceInfo all the device information struct, maybe some tencent need, some baidu need
// we store together, just use what you need
type DeviceInfo struct {
	DeviceID     string
	Imei         string
	WifiMac      string
	Mac          string
	Brand        string
	Manufacturer string
	Model        string
	Product      string
	Hardware     string
	Device       string
	OsVersion    string
	AppVersion   string
	Random       string
	Board        string
}
