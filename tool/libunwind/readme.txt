1 refer https://www.nongnu.org/libunwind/
2 Android也是用这个库来打印调用堆栈


how to build
1 ./configure --host=arm-ca53-linux-uclibcgnueabihf 其中后面的编译器取决于你自己的编译器
2 直接make 会出现execinfo.h没有定义的问题，看起来只有pc的gcc才有这个头文件
其他交叉编译的并没有，修改Makefile，去掉tests的依赖

#am__append_15 = tests

3 make 即可得到src/.libs/下面的库

4 参考debuggerd.cpp 和backtrace.tgz（from Andoird） 来方便在linux上得到堆栈

可以调用dump_current_backtrace/dump_backtrace
得到类似如下的堆栈：


I 2019-10-16 19:05:03.157606 [5063]/Backtrace:   #00 pc 0000ec10  /lib/libuClibc-1.0.17.so (__write_nocancel+8)

I 2019-10-16 19:05:03.157943 [5063]/Backtrace:   #01 pc 0000ec7c  /lib/libuClibc-1.0.17.so (write+68)

I 2019-10-16 19:05:03.158082 [5063]/Backtrace:   #02 pc 0002b9b0  /lib/libuClibc-1.0.17.so (__stdio_WRITE+128)


TODO：
使用addr2line取前面的地址，似乎不太准？

