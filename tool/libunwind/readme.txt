1 refer https://www.nongnu.org/libunwind/
2 Android也是用这个库来打印调用堆栈


how to build
1 ./configure --host=arm-ca53-linux-uclibcgnueabihf 其中后面的编译器取决于你自己的编译器
2 直接make 会出现execinfo.h没有定义的问题，看起来只有pc的gcc才有这个头文件
其他交叉编译的并没有，修改Makefile，去掉tests的依赖

#am__append_15 = tests

3 make 即可得到src/.libs/下面的库
