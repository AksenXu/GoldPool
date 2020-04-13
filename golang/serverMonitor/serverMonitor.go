package serverMonitor

import (
	"bufio"
	"fmt"
	"linkdata/api"
	"linkdata/pkg/define"
	"linkdata/pkg/glog"
	"linkdata/pkg/tcpserver"
	"os"
	"strconv"
	"time"
)

var mainmenu int

type funcHandler func(menu int)

// Menu debug menu
type Menu struct {
	MenuContent   string
	MenuNum       int
	ParentMenuNum int
	Funhandler    funcHandler
}

var MonitorMenu [100]Menu
var MenuNumbers int

func findMenu(num int) *Menu {
	for i := 0; i < MenuNumbers; i++ {
		if MonitorMenu[i].MenuNum == num {
			return &MonitorMenu[i]
		}
	}
	fmt.Printf("findMenu failed num = %v\n", num)
	return nil
}

func backTopMenu(num int) {
	menu := findMenu(num)
	if menu == nil {
		fmt.Printf("backTopMenu failed num: %v\n", num)
		mainmenu = 0
	} else {
		mainmenu = menu.ParentMenuNum
	}
}

func printSubMenu(parentnum int) {
	fmt.Printf("Please Choose the Menu:\n")
	for i := 0; i < MenuNumbers; i++ {
		if MonitorMenu[i].ParentMenuNum == parentnum {
			fmt.Printf("%v: %v\n", MonitorMenu[i].MenuNum, MonitorMenu[i].MenuContent)
		}
	}
}

func printServerStatus(num int) {
	fmt.Printf("\nprintServerStatus:\n")
	//connect gateserver for server status
	req := api.F2GReportStatReq{
		KnownPublicAddr: "",
		ConnCount:       0,
	}
	var resp api.G2FReportStatResp

	for {
		c := tcpserver.DailForever(define.GateServerPublicAddr, "public gateserver")
		err := c.SendAndReply(uint16(api.ServerCmdID_Cmd_M2GStatusReq), &req, &resp)
		c.Close()
		if err == nil {
			break
		}
		glog.Warningf("connect public gateserver error: %v, retry later", err)
		time.Sleep(time.Second * 1)
	}
}

func traceOrder(num int) {
	fmt.Printf("traceOrder %v\n", num)
}

func simulateCapture(num int) {
	fmt.Printf("simulateCapture %v\n", num)
}

func traceDevice(num int) {
	fmt.Printf("traceDevice %v\n", num)
}

func initMenu() {
	MonitorMenu[0] = Menu{"Back To Upper Level.", 0, 0, backTopMenu}
	MonitorMenu[1] = Menu{"Get Server Status.", 1, 0, printServerStatus}
	MonitorMenu[2] = Menu{"Trace the Capture Order.", 2, 0, traceOrder}
	MonitorMenu[3] = Menu{"Simulate the Capture Action.", 3, 0, simulateCapture}
	MonitorMenu[4] = Menu{"Trace the Device Status.", 4, 0, traceDevice}
	MenuNumbers = 5
}

// Main monitor tool entry function
// pop some menu to get the trace info
func Main() {
	fmt.Printf("monitortool stared: %v\n", time.Now())
	initMenu()

	for {
		printSubMenu(mainmenu)

		fmt.Println("请选择相应菜单：\n")
		//读键盘 以换行符结束
		reader := bufio.NewReader(os.Stdin)
		str, _ := reader.ReadString('\n')

		fmt.Printf("Got input: %v\n", str)
		menunum, err := strconv.Atoi(str[:len(str)-1])
		if err != nil {
			fmt.Printf("Error input %v\n", str)
			continue
		}
		menu := findMenu(menunum)
		if menu == nil {
			continue
		}

		if menu.Funhandler != nil {
			menu.Funhandler(menunum)
		} else {
			mainmenu = menunum
		}
	}
}
