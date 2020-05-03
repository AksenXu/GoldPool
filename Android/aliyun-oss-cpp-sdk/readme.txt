如何编译新版的aliyun-oss-sdk?

1 git clone https://github.com/aliyun/aliyun-oss-cpp-sdk.git
2 将sdk目录改为jni，方便直接使用ndk-build，而不使用默认的cmake
3 将Android.mk Application.mk放入jni目录，执行ndk-build即可