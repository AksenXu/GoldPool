#!/bin/sh
#input:      
#            1: ���̵�map��Ϣ    cat /proc/xxx/maps > maps.txt
#            2�������ڴ�й©�Ķ�ջ echo "0xb30d550e, 0xb6e63376, 0xb692eb1c, 0xb56401ae, 0xb56401ec, 0xb5640260, 0xb323a242, 0xb3245ba4, 0xb3235618, 0xb44adf5a, 0xb44a80d4, 0xb6815c36, 0xb682452c, 0xb6824dc2, 0xb6824e6c, 0xb6925cc0, 0xb6e67642, 0xb6e655f6" > stack.txt
#            3: �ļ�ϵͳ��������·�� rootfs: such as ~/MTK/MT6580/trunk/out/target/product/demo
#���mediaserver���̵��ڴ�й©������ʹ�����������mediaserver���̵��ڴ�debug
#libc.debug.malloc=1
#libc.debug.malloc.program=mediaserver
#ʹ����������
#dumpsys media.player -m
#���õ���ջ��Ϣ

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

#����
#b6f4e000-b6f4f000 rw-p 00001000 b3:13 1091       /system/lib/libdirect-coredump.so
#�õ������ʼ��ַ b6f4e000, ������ַb6f4f000���Լ��������/system/lib/libdirect-coredump.so
#�õ�����map����Ϣ
#ò��script���ô洢�ɽṹ�壬�Ǿ�ÿ������һ�飬Ч�ʱȽϵ�
readMapToMemory()
{
    return
}

#����һ����ַ����0xb3245ba4
#��readMapToMemory�����ҵ���Ӧ�Ŀ�
#b3228000-b325d000 r-xp 00000000 b3:13 1171       /system/lib/libimageio_plat_drv.so
#����ƫ�Ƶ�ַ����b3245ba4 - b3228000 = 1DBA4
#~/MTK/MT6580/trunk$ arm-eabi-addr2line -Cfe out/target/product/joyasz6580_weg_l/symbols/system/lib/libimageio_plat_drv.so 1DBA4
#std::list<stISP_BUF_INFO, std::allocator<stISP_BUF_INFO> >::push_back(stISP_BUF_INFO const&)
#trunk/./external/stlport/stlport/stl/_list.h:509
#�Ϳ��Եõ���Ӧ��Դ��������
findAddrInLib()
{
    OLD_IFS=$IFS
    IFS=$'\r\n'
    targetAddr=${1:2:8}   #�ص�0x��16������ʼ���ź�ĩβ���ܴ��ڵ�\r\n, ֻ����8���ַ�
    #echo "libraryname: $4"
    #echo "Library Range: $2-$3"
    #echo "TargetAddr: $targetAddr"
    
    #תΪ����
    let targetAddr=$((16#${targetAddr}))
    let startAddr=$((16#${2}))
    let endAddr=$((16#${3}))
    
    if [ $targetAddr -lt $startAddr ] || [ $targetAddr -gt $endAddr ]; then
        #targetAddr���ڵ�ǰ��ĵ�ַ��Χ�ռ䣬�˳�
        return 0
    fi
    
    #echo targetAddr=$targetAddr
    #echo startAddr=$startAddr
    #echo endAddr=$endAddr
    
    #�õ���ַ�ڵ�ǰ�⵱�еı�����
    offset=`expr $targetAddr - $startAddr`
    
    #תΪ16����
    offsetHex=`printf %x $offset`

    #��ľ���·��
    path=`echo -n $ROOTPATH$4`
    
    #�õ�offset��Ӧ��sourcecode

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

                #expr������ٶȷǳ���
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

#����map�ļ����ڴ浱��
#ò��shellû�кõĴ洢�ṹ���ǻ���ÿ�ζ��ֶ���
#readMapToMemory

IFS=$'\n'  #����ո�ᱻ�ֿ��ɲ�ͬ��line
for line in `cat $STACKFILE`
do
    echo "callstack = $line"
    #size   947608, dup    1, 0xb30d550e, 0xb6e63376, 0xb692eb1c, 0xb56401ae, 0xb56401ec, 0xb5640260, 0xb323a242, 0xb3245ba4, 0xb3235618, 0xb44adf5a, 0xb44a80d4, 0xb6815c36, 0xb682452c, 0xb6824dc2, 0xb6824e6c, 0xb6925cc0, 0xb6e67642, 0xb6e655f6
    OLD_IFS=$IFS
    IFS=", \r\n" #�ָ�ɲ�ͬ�ĵ�ַ

    for addr in $line
    do
        #echo $addr
        match=`expr match "$addr" "0x"`
        
        #����ֻ����0x��ͷ�ĵ�ַ������������
        if [ $match = 0 ]; then
            echo "Ignore not address: $addr"
            continue
        fi
        findAddrInMap $addr
    done
    IFS=$OLD_IFS
done