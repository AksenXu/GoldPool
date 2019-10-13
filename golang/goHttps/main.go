package main

import (
	"fmt"
    "net/http"
)

// https://blog.csdn.net/wangshubo1989/article/details/77508738
// openssl genrsa -out server.key 2048
// openssl req -new -x509 -sha256 -key server.key -out server.crt -days 3650
func handler(w http.ResponseWriter, r *http.Request) {
	pb2, err := ioutil.ReadAll(r.Body)
	if err != nil {
		fmt.Printf("xxxx")
	}
	
    fmt.Fprintf(w,
        "Hi, This is an example of https service in golang!")
}

func main() {
    http.HandleFunc("/", handler)
    http.ListenAndServeTLS(":8081", "server.crt",
                           "server.key", nil)
}
