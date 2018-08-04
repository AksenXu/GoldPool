#!/bin/sh
#input:      
#            1: 进程的map信息    cat /proc/xxx/maps > maps.txt
#            2：怀疑内存泄漏的堆栈 echo "0xb30d550e, 0xb6e63376, 0xb692eb1c, 0xb56401ae, 0xb56401ec, 0xb5640260, 0xb323a242, 0xb3245ba4, 0xb3235618, 0xb44adf5a, 0xb44a80d4, 0xb6815c36, 0xb682452c, 0xb6824dc2, 0xb6824e6c, 0xb6925cc0, 0xb6e67642, 0xb6e655f6" > stack.txt
#            3: 文件系统编译的输出路径 rootfs: such as ~/MTK/MT6580/trunk/out/target/product/demo
#针对mediaserver进程的内存泄漏，首先使用以下命令打开mediaserver进程的内存debug
#libc.debug.malloc=1
#libc.debug.malloc.program=mediaserver
#使用以下命令
#dumpsys media.player -m
#来得到堆栈信息

#usage:      getTrace.sh    maps.txt stack.txt rootfsPath

if [ $# != 3 ]; then
    echo "usage: $0 maps.txt stack.txt rootfsPath"
    exit
fi

MAPFILE=$1
STACKFILE=$2
ROOTPATH=$3
OLD_IFS=$IFS

echo "MAPFILE=$MAPFILE STACKFILE=$STACKFILE ROOTPATH=$ROOTPATH"

#例如
#b6f4e000-b6f4f000 rw-p 00001000 b3:13 1091       /system/lib/libdirect-coredump.so
#得到库的起始地址 b6f4e000, 结束地址b6f4f000，以及库的名字/system/lib/libdirect-coredump.so
#得到整个map的信息
#貌似script不好存储成结构体，那就每个都读一遍，效率比较低
readMapToMemory()
{
    return
}

#输入一个地址例如0xb3245ba4
#从readMapToMemory里面找到对应的库
#b3228000-b325d000 r-xp 00000000 b3:13 1171       /system/lib/libimageio_plat_drv.so
#这样偏移地址就是b3245ba4 - b3228000 = 1DBA4
#~/MTK/MT6580/trunk$ arm-eabi-addr2line -Cfe out/target/product/joyasz6580_weg_l/symbols/system/lib/libimageio_plat_drv.so 1DBA4
#std::list<stISP_BUF_INFO, std::allocator<stISP_BUF_INFO> >::push_back(stISP_BUF_INFO const&)
#trunk/./external/stlport/stlport/stl/_list.h:509
#就可以得到对应的源代码内容
findAddrInLib()
{
    OLD_IFS=$IFS
    IFS=$'\r\n'
    targetAddr=${1:2:8}   #截掉0x的16进制起始符号和末尾可能存在的\r\n, 只保留8个字符
    #echo "libraryname: $4"
    #echo "Library Range: $2-$3"
    #echo "TargetAddr: $targetAddr"
    
    #转为数字
    let targetAddr=$((16#${targetAddr}))
    let startAddr=$((16#${2}))
    let endAddr=$((16#${3}))
    
    if [ $targetAddr -lt $startAddr ] || [ $targetAddr -gt $endAddr ]; then
        #targetAddr不在当前库的地址范围空间，退出
        return 0
    fi
    
    #echo targetAddr=$targetAddr
    #echo startAddr=$startAddr
    #echo endAddr=$endAddr
    
    #得到地址在当前库当中的便宜量
    offset=`expr $targetAddr - $startAddr`
    
    #转为16进制
    offsetHex=`printf %x $offset`

    #库的绝对路径
    path=`echo -n $ROOTPATH$4`
    
    #得到offset对应的sourcecode

    echo path=$path offsetHex=$offsetHex
    addr2line -Cfe $path $offsetHex
    echo
    echo
    IFS=$OLD_IFS
    return 1
}

findAddrInMap()
{
    echo "findAddrInMap: targetAddr $1"
    OLD_IFS=$IFS
    IFS=$'\n'
    for line in `cat $MAPFILE`
    do
        #echo "mapline = $line"
        #b321f000-b3226000 r-xp 00000000 b3:13 947        /system/lib/libJpgDecPipe.so
        IFS=": -"
        startAddr=0
        endAddr=0
        libName=""
        for addr in $line
            do
                if [ $startAddr = 0 ]; then 
                    startAddr=$addr 
                    continue 
                fi
                if [ $endAddr = 0 ]; then 
                    endAddr=$addr 
                    continue 
                fi

                #expr处理的速度非常慢
                #match=`expr match "$addr" "/system"`
                #if [ $match != 0 ]; then 
                if [[ "$addr" == /system* ]]; then
                    libName=$addr 
                    #echo startAddr=$startAddr
                    #echo endAddr=$endAddr
                    #echo libName=$libName
                    findAddrInLib $1 $startAddr $endAddr $libName
                    if [ $? -eq 1 ]; then
                        return
                    fi
                fi
            
            done
    done
    
    IFS=$OLD_IFS
    return
}

#加载map文件到内存当中
#貌似shell没有好的存储结构，那还是每次都现读吧
#readMapToMemory

IFS=$'\n'  #否则空格会被分开成不同的line
for line in `cat $STACKFILE`
do
    echo "callstack = $line"
    #size   947608, dup    1, 0xb30d550e, 0xb6e63376, 0xb692eb1c, 0xb56401ae, 0xb56401ec, 0xb5640260, 0xb323a242, 0xb3245ba4, 0xb3235618, 0xb44adf5a, 0xb44a80d4, 0xb6815c36, 0xb682452c, 0xb6824dc2, 0xb6824e6c, 0xb6925cc0, 0xb6e67642, 0xb6e655f6
    OLD_IFS=$IFS
    IFS=", \r\n" #分割成不同的地址

    for addr in $line
    do
        #echo $addr
        match=`expr match "$addr" "0x"`
        
        #我们只处理0x开头的地址，其他不处理
        if [ $match = 0 ]; then
            echo "Ignore not address: $addr"
            continue
        fi
        findAddrInMap $addr
    done
    IFS=$OLD_IFS
done