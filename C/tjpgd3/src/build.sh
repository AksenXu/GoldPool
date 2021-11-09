gcc -fPIC -shared -o libtjpg.so tjpgd.c
gcc -o tjpeg_test test.c -L. -ltjpg

