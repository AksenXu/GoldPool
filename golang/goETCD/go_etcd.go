package main

import (
	"context"
	"fmt"
	"time"

	"go.etcd.io/etcd/clientv3"
)

// https://blog.csdn.net/q282176713/article/details/81177545
// https://github.com/etcd-io/etcd
// git clone https://github.com/coreos/etcd.git
// 注意使用etcd3.x 的版本，确认系统版本：
// etcd -v 
func main() {

	cli, err := clientv3.New(clientv3.Config{
		// Endpoints: []string{"localhost:2379", "localhost:22379", "localhost:32379"},
		Endpoints:   []string{"localhost:2379"},
		DialTimeout: 5 * time.Second,
	})
	if err != nil {
		fmt.Println("connect failed, err:", err)
		return
	}

	fmt.Println("connect succ")

	defer cli.Close()
	//设置1秒超时，访问etcd有超时控制
	t1 := time.Now()
	ctx, _ := context.WithTimeout(context.Background(), time.Second)
	//操作etcd
	_, err = cli.Put(ctx, "key", "v")
	//操作完毕，取消etcd
	// cancel()

	t2 := time.Now()
	fmt.Println("put耗时", t2.Sub(t1))
	if err != nil {
		fmt.Println("put failed, err:", err)
		return
	}
	//取值，设置超时为1秒
	ctx, _ = context.WithTimeout(context.Background(), 10*time.Second)
	t1 = time.Now()
	resp, err := cli.Get(ctx, "key")
	fmt.Println("get 耗时:", time.Now().Sub(t1))
	// 	cancel()
	if err != nil {
		fmt.Println("get failed, err:", err)
		return
	}
	for _, ev := range resp.Kvs {
		fmt.Printf("%s : %s\n", ev.Key, ev.Value)
	}

	//测试redis
}
