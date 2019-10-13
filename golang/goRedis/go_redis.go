// https://redis.io/download
// $ wget http://download.redis.io/releases/redis-5.0.5.tar.gz
// $ tar xzf redis-5.0.5.tar.gz
// $ cd redis-5.0.5
// $ make

// sudo apt-get install redis-server
// redis-server
// redis-cli

// go get github.com/garyburd/redigo/redis


// https://www.cnblogs.com/wdliu/p/9330278.html
package main

import (
	"fmt"

	"github.com/garyburd/redigo/redis"
)

func main() {
	conn, err := redis.Dial("tcp", "127.0.0.1:6379")
	if err != nil {
		fmt.Println("connect redis error :", err)
		return
	}
	defer conn.Close()
	_, err = conn.Do("SET", "name", "wd")
	if err != nil {
		fmt.Println("redis set error:", err)
	}
	name, err := redis.String(conn.Do("GET", "name"))
	if err != nil {
		fmt.Println("redis get error:", err)
	} else {
		fmt.Printf("Got name: %s \n", name)
	}
}
