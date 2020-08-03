0 ./build.sh
1 sudo insmod logdriver/logger.ko
2 sudo chmod 666 /dev/log_*
3 export LD_LIBRARY_PATH=`pwd`/liblog
4 ./logcat/logcat --test
5 ./logcat/logcat
will print:
--------- beginning of /dev/log_main
I/        (10550): ++++++++++++++++++++++++++++++++++++++++++++++++++++
I/        (10550): start output some log for debug
I/        (10550): ++++++++++++++++++++++++++++++++++++++++++++++++++++

6 sudo rmmod logger 